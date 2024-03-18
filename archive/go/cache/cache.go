package main

import (
	"sort"
	"strconv"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

type Cache interface {
	SetMaxMemory(size string) bool
	Set(key string, val interface{}, expire time.Duration)
	Get(key string) (interface{}, bool)
	Del(key string) bool
	Exists(key string) bool
	Flush() bool
	Keys() int64
}

type cache struct {
	maxMemory int64
	memory    int64
	// 过期使用惰性删除,num实际上可能不准确
	num      int64
	capacity int64

	rehashs struct {
		sync.Mutex
		enable int32
		index  int
	}

	hashtable [2]*hashtable

	// 淘汰池
	evictionPool struct {
		sync.Mutex
		e []*entry
	}
}

func NewCache() Cache {
	return newCache(1024)
}

func newCache(cap int64) *cache {
	c := &cache{
		capacity:  cap,
		hashtable: [2]*hashtable{newHashtable(cap)},
		evictionPool: struct {
			sync.Mutex
			e []*entry
		}{e: make([]*entry, 0, 20)},
		rehashs: struct {
			sync.Mutex
			enable int32
			index  int
		}{enable: 0, index: 0},
	}
	c.SetMaxMemory("1GB")
	return c
}

func (c *cache) SetMaxMemory(size string) bool {
	if len(size) < 3 {
		return false
	}
	num := size[:len(size)-2]
	unit := size[len(size)-2:]
	maxMemory, err := strconv.ParseInt(num, 10, 64)
	if err != nil {
		return false
	}
	if maxMemory == 0 {
		return false
	}
	switch strings.ToUpper(unit) {
	case "KB":
		c.maxMemory = maxMemory << 10
	case "MB":
		c.maxMemory = maxMemory << 20
	case "GB":
		c.maxMemory = maxMemory << 30
	default:
		return false
	}
	return true
}

func (c *cache) Set(key string, val interface{}, expire time.Duration) {
	c.rehashs.Lock()
	defer c.rehashs.Unlock()
	if c.rehashs.enable == 0 {
		if float64(c.num)/float64(c.capacity) > 0.75 {
			// 扩容一倍,渐进式rehash
			atomic.AddInt64(&c.capacity, c.capacity)
			c.hashtable[1] = newHashtable(c.capacity)
			atomic.StoreInt32(&c.rehashs.enable, 1)
			c.hashtable[1].Set(key, val, expire)
		} else {
			c.hashtable[0].Set(key, val, expire)
		}
	} else {
		if c.rehashs.enable == 0 {
			c.Set(key, val, expire)
			return
		}
		c.hashtable[1].Set(key, val, expire)
		c.rehash()
	}
	atomic.AddInt64(&c.num, 1)
	atomic.AddInt64(&c.memory, int64(sizeof(val)))
	for c.memory > c.maxMemory && c.num > 1 {
		//清理内存
		c.lru()
		if len(c.evictionPool.e) == 0 {
			break
		}
	}
}

func (c *cache) rehash() {
	// rehash的时候使用锁
	e, comple := c.hashtable[0].index(c.rehashs.index)
	if comple {
		//完成
		c.hashtable[0], c.hashtable[1] = c.hashtable[1], nil
		c.rehashs.enable = 0
		c.rehashs.index = 0
		return
	}
	c.rehashs.index++
	if e == nil {
		c.rehash()
		return
	}
	for e != nil {
		if e.expired() {
			// 过期删除,但是这里没有对实际内存删除
			atomic.AddInt64(&c.num, -1)
			atomic.AddInt64(&c.memory, -int64(sizeof(e.value)))
		} else {
			c.hashtable[1].SetExpireTime(e.key, e.value, e.expireTime)
		}
		e, e.next = e.next, nil
	}
}

// 近似lru清理内存
func (c *cache) lru() {
	c.evictionPool.Lock()
	defer c.evictionPool.Unlock()
	if c.rehashs.enable == 0 {
		// 取样20个
		if len(c.evictionPool.e) <= 15 {
			m := make(map[string]struct{})
			for _, v := range c.evictionPool.e {
				m[v.key] = struct{}{}
			}
			e := c.hashtable[0].random(5, -1)
			for _, v := range e {
				if _, ok := m[v.key]; ok {
					continue
				}
				c.evictionPool.e = append(c.evictionPool.e, v)
			}
			sort.Slice(c.evictionPool.e, func(i, j int) bool {
				return c.evictionPool.e[i].operateTime.Before(c.evictionPool.e[j].operateTime)
			})
		}
		e := c.evictionPool.e[0]
		c.Del(e.key)
		c.evictionPool.e = c.evictionPool.e[1:]
		if len(c.evictionPool.e) == 0 {
			return
		}
	} else {
		// TODO: 扩容中进行
		// 好像有点麻烦,需要根据rehash的进度,从未rehash的地方进行取样,这样就得加锁,为了get读等其它的性能又不想加锁
		// 或者只有set操作的时候才进行清理,(set加了一个rehash锁)
	}
}

func (c *cache) Get(key string) (interface{}, bool) {
	if c.rehashs.enable == 0 {
		val, err := c.hashtable[0].Get(key)
		if err != nil {
			if err == ExpiredErr {
				// 过期删除
				atomic.AddInt64(&c.num, -1)
				atomic.AddInt64(&c.memory, -int64(sizeof(val)))
			}
			return nil, false
		}
		return val, true
	} else {
		c.rehashs.Lock()
		defer c.rehashs.Unlock()
		if c.rehashs.enable == 0 {
			return c.Get(key)
		}
		// rehash中 先从1中查,未查到再从0查
		val, err := c.hashtable[1].Get(key)
		if err != nil {
			if err == ExpiredErr {
				// 过期删除
				atomic.AddInt64(&c.num, -1)
				atomic.AddInt64(&c.memory, -int64(sizeof(val)))
				return nil, false
			}
			val, err = c.hashtable[0].Get(key)
			if err != nil {
				if err == ExpiredErr {
					// 过期删除
					atomic.AddInt64(&c.num, -1)
					atomic.AddInt64(&c.memory, -int64(sizeof(val)))
				}
				return nil, false
			}
		}
		c.rehash()
		return val, true
	}
}

func (c *cache) Del(key string) bool {
	if c.rehashs.enable == 0 {
		val, ok := c.hashtable[0].Del(key)
		if !ok {
			return false
		}
		atomic.AddInt64(&c.memory, -int64(sizeof(val)))
		atomic.AddInt64(&c.num, -1)
		return true
	} else {
		// rehash中
		c.rehashs.Lock()
		defer c.rehashs.Unlock()
		if c.rehashs.enable == 0 {
			return c.Del(key)
		}
		val, ok := c.hashtable[0].Del(key)
		if !ok {
			val, ok = c.hashtable[1].Del(key)
			if !ok {
				return false
			}
		}
		atomic.AddInt64(&c.memory, -int64(sizeof(val)))
		atomic.AddInt64(&c.num, -1)
		return true
	}
}

func (c *cache) Exists(key string) bool {
	_, ok := c.Get(key)
	return ok
}

func (c *cache) Flush() bool {
	if c.rehashs.enable == 0 {
		c.hashtable[0].flush(&c.memory, &c.num)
	} else {
		// rehash中
		c.rehashs.Lock()
		defer c.rehashs.Unlock()
		c.hashtable[0].flush(&c.memory, &c.num)
		c.hashtable[1].flush(&c.memory, &c.num)
	}
	return true
}

// Keys 过期使用惰性删除,keys可能并不准确
func (c *cache) Keys() int64 {
	return c.num
}
