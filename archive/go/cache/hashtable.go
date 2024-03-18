package main

import (
	"errors"
	"hash/fnv"
	"math/rand"
	"sync"
	"sync/atomic"
	"time"
)

type entry struct {
	key         string
	value       interface{}
	expireTime  time.Time
	operateTime time.Time
	next        *entry
}

var ExpiredErr = errors.New("key过期")

func (v *entry) expired() bool {
	return time.Now().After(v.expireTime)
}

// 过期也会返回value,用于计算内存占用大小
func (v *entry) val() (interface{}, error) {
	if !v.expired() {
		return v.value, nil
	}
	return v.value, ExpiredErr
}

// 拉链法实现hashtable
type hashtable struct {
	//maxMemory int64
	//memory    int64
	//
	//cap int64
	//num int64

	// 分段锁提高性能
	bucket []*bucket
}

func newHashtable(cap int64) *hashtable {
	ht := &hashtable{
		//maxMemory: maxMemory, memory: memory, cap: cap, num: num,
		bucket: make([]*bucket, 0, cap/1024),
	}
	for i := int64(0); i < (cap / 1024); i++ {
		ht.bucket = append(ht.bucket, &bucket{})
	}
	return ht
}

// 计算key 返回桶位置和数组位置
func (h *hashtable) hash(key string) (int, int) {
	hash := fnv.New64()
	hash.Write([]byte(key))
	nk := int(hash.Sum64())
	if nk < 0 {
		nk = -nk
	}
	return nk % len(h.bucket), nk % 1024
}

func (h *hashtable) Get(key string) (interface{}, error) {
	b, n := h.hash(key)
	return h.bucket[b].get(key, n)
}

func (h *hashtable) Set(key string, val interface{}, expire time.Duration) {
	b, n := h.hash(key)
	h.bucket[b].set(key, n, val, expire)
}

func (h *hashtable) SetExpireTime(key string, val interface{}, expireTime time.Time) {
	b, n := h.hash(key)
	h.bucket[b].setExpireTime(key, n, val, expireTime)
}

func (h *hashtable) Del(key string) (interface{}, bool) {
	b, n := h.hash(key)
	val, ok := h.bucket[b].del(key, n)
	return val, ok
}

// flush 不在hashtab层加锁,循环桶清空
func (h *hashtable) flush(mem, num *int64) {
	for _, v := range h.bucket {
		v.flush(mem, num)
	}
}

func (h *hashtable) index(index int) (*entry, bool) {
	b, n := index/1024, index%1024
	if index >= len(h.bucket)*1024 {
		return nil, true
	}
	return h.bucket[b].getAndDel(n), false
}

//从某个位置开始随机取值,-1为随机所有key
func (h *hashtable) random(count, start int) []*entry {
	if start == -1 {
		start = rand.Intn(len(h.bucket) * 1024)
	}
	startBucket := start / 1024
	randomBucket := startBucket + rand.Intn(len(h.bucket)-startBucket)
	startIndex := 0
	if randomBucket == startBucket {
		startIndex = start % 1024
		startIndex = startIndex + rand.Intn(1024-startIndex)
	} else {
		startBucket = randomBucket
		startIndex = rand.Intn(1024)
	}
	ret := make([]*entry, 0, count)
	for i := 0; i < 1024; i++ {
		var e *entry
		if startIndex+i >= 1024 {
			// 跨过桶了
			b := h.bucket[(startBucket+1)%len(h.bucket)]
			e = b.index((startIndex + i) % 1024)
		} else {
			e = h.bucket[startBucket].index(startIndex + i)
		}
		if e == nil {
			continue
		}
		for {
			ret = append(ret, e)
			if e.next == nil || len(ret) >= count {
				break
			}
			e = e.next
		}
		if len(ret) >= count {
			break
		}
	}
	return ret
}

type bucket struct {
	sync.RWMutex
	kv [1024]*entry
	// 桶内也对内存 num进行统计,另外一个优化思路,定时对桶内的内存和数量进行统计 然后进行内存释放和扩容
	mem int64
	num int64
}

// 清理,对总的内存和数量进行计算
func (b *bucket) flush(mem, num *int64) {
	b.Lock()
	defer b.Unlock()
	b.kv = [1024]*entry{}
	atomic.AddInt64(mem, -b.mem)
	atomic.AddInt64(num, -b.num)
	b.mem = 0
	b.num = 0
}

func (b *bucket) getAndDel(n int) (v *entry) {
	b.Lock()
	defer b.Unlock()
	v, b.kv[n] = b.kv[n], nil
	return
}

// 获取指针
func (b *bucket) getP(key string, n int) (*entry, *entry) {
	v := b.kv[n]
	prev := v
	for v != nil {
		if v.key == key {
			return v, prev
		}
		prev = v
		v = v.next
	}
	return nil, nil
}

func (b *bucket) index(n int) *entry {
	b.RLock()
	defer b.RUnlock()
	return b.kv[n]
}

func (b *bucket) get(key string, n int) (interface{}, error) {
	b.RLock()
	v, prev := b.getP(key, n)
	if v == nil {
		b.RUnlock()
		return nil, errors.New("key不存在")
	}
	val, err := v.val()
	if err != nil {
		b.RUnlock()
		b.Lock()
		defer b.Unlock()
		b.mem -= int64(sizeof(val))
		b.num--
		// 过期删除
		if prev == v {
			b.kv[n] = nil
		} else {
			prev.next = v.next
		}
		return val, err
	}
	v.operateTime = time.Now()
	b.RUnlock()
	return val, nil
}

func (b *bucket) setExpireTime(key string, n int, val interface{}, expireTime time.Time) {
	b.Lock()
	defer b.Unlock()
	v := b.kv[n]
	b.mem += int64(sizeof(val))
	b.num++
	if v == nil {
		b.kv[n] = &entry{
			key:         key,
			value:       val,
			expireTime:  expireTime,
			operateTime: time.Now(),
			next:        nil,
		}
		return
	}
	for v != nil {
		if v.key == key {
			v.value = val
			v.expireTime = expireTime
			v.operateTime = time.Now()
			return
		}
		if v.next == nil {
			break
		}
		v = v.next
	}
	v.next = &entry{
		key:         key,
		value:       val,
		expireTime:  expireTime,
		operateTime: time.Now(),
		next:        nil,
	}
}

func (b *bucket) set(key string, n int, val interface{}, expire time.Duration) {
	expireTime := time.Now()
	if expire == 0 {
		// 直接设置一个超大的时间
		expireTime = expireTime.Add(time.Hour * 24 * 365 * 10)
	} else {
		expireTime = expireTime.Add(expire)
	}
	b.setExpireTime(key, n, val, expireTime)
}

// 删除并返回删除的值
func (b *bucket) del(key string, n int) (interface{}, bool) {
	b.Lock()
	defer b.Unlock()
	v, prev := b.getP(key, n)
	if v == nil {
		return nil, false
	}
	b.mem -= int64(sizeof(v.value))
	b.num--
	if prev == v {
		b.kv[n] = nil
	} else {
		prev.next = v.next
	}
	return v.value, true
}
