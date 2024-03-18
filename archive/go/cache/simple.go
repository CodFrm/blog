package main

import (
	"hash/fnv"
	"strconv"
	"strings"
	"sync"
	"time"
)

type lru struct {
	prev *lru
	next *lru
	key  string
}

type simpleCache struct {
	sync.Mutex

	maxMemory int
	memory    int

	hashtable []*entry

	lruhead, lrutail *lru
	lruHash          map[string]*lru

	num int
	cap int
}

func NewSimpleCache() Cache {
	return newSimpleCache()
}

func newSimpleCache() *simpleCache {
	c := &simpleCache{
		hashtable: make([]*entry, 1024),
		cap:       1024,
		lruhead:   &lru{},
		lrutail:   &lru{},
		lruHash:   map[string]*lru{},
	}
	c.lruhead.next = c.lrutail
	c.lrutail.prev = c.lruhead
	go func() {
		// 扫描过期
		for {
			time.Sleep(time.Minute)
			c.Lock()
			for n, p := range c.hashtable {
				prev := p
				for p != nil {
					if p.expired() {
						if p == c.hashtable[n] {
							c.hashtable[n] = p.next
						} else {
							prev.next = p.next
						}
						c.num--
						c.memory -= sizeof(p.value)
					}
					prev = p
					p = p.next
				}
			}
			c.Unlock()
		}
	}()
	return c
}

func (c *simpleCache) SetMaxMemory(size string) bool {
	if len(size) < 3 {
		return false
	}
	num := size[:len(size)-2]
	unit := size[len(size)-2:]
	maxMemory, err := strconv.Atoi(num)
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

func (c *simpleCache) hash(key string) int {
	hash := fnv.New64()
	hash.Write([]byte(key))
	nk := int(hash.Sum64())
	if nk < 0 {
		nk = -nk
	}
	return nk % c.cap
}

func (c *simpleCache) rehashkey(cap int, key string) int {
	hash := fnv.New64()
	hash.Write([]byte(key))
	nk := int(hash.Sum64())
	if nk < 0 {
		nk = -nk
	}
	return nk % cap
}

func (c *simpleCache) rehash() {
	//扩容
	newCap := c.cap * 2
	tmpHashtable := make([]*entry, newCap)
	for _, p := range c.hashtable {
		for p != nil {
			n := c.rehashkey(newCap, p.key)
			nv := tmpHashtable[n]
			nk := &entry{
				key:         p.key,
				value:       p.value,
				expireTime:  p.expireTime,
				operateTime: p.operateTime,
				next:        nil,
			}
			if nv == nil {
				tmpHashtable[n] = nk
			} else {
				for nv.next != nil {
					nv = nv.next
				}
				nv.next = nk
			}
			p = p.next
		}
	}
	c.cap = newCap
	c.hashtable = tmpHashtable
}

func (c *simpleCache) Set(key string, val interface{}, expire time.Duration) {
	c.Lock()
	defer c.Unlock()
	defer c.lru(key)

	c.memory += sizeof(val)

	if float32(c.num)/float32(c.cap) > 0.75 {
		c.rehash()
	}

	n := c.hash(key)
	expireTime := time.Now()
	if expire == 0 {
		// 设置一个很大的时间
		expireTime = expireTime.Add(time.Hour * 24 * 365 * 10)
	} else {
		expireTime = expireTime.Add(expire)
	}
	p := c.hashtable[n]
	e := &entry{
		key:        key,
		value:      val,
		expireTime: expireTime,
		next:       nil,
	}
	if p == nil {
		c.hashtable[n] = e
		c.num++
		return
	}
	prev := p
	for p != nil {
		if p.key == key {
			p.value = val
			p.expireTime = expireTime
			return
		}
		prev = p
		p = p.next
	}
	prev.next = e
	c.num++
}

func (c *simpleCache) lru(key string) {
	l := c.lruHash[key]
	if l == nil {
		// 插入头
		nl := &lru{prev: c.lruhead, next: c.lruhead.next, key: key}
		c.lruhead.next.prev = nl
		c.lruhead.next = nl
		c.lruHash[key] = nl
	} else {
		// 移动到头
		l.prev.next = l.next
		l.next.prev = l.prev
		l.prev = c.lruhead
		l.next = c.lruhead.next
		c.lruhead.next.prev = l
		c.lruhead.next = l
	}
	// 清理内存
	for c.memory > c.maxMemory && c.num > 1 {
		if c.lrutail.prev == c.lruhead {
			return
		}
		rmk := c.lrutail.prev
		c.lrutail.prev.next = c.lrutail
		c.lrutail.prev = c.lrutail.prev.prev
		c.del(rmk.key)
	}
}

func (c *simpleCache) delLru(key string) {
	l := c.lruHash[key]
	if l != nil {
		l.prev.next = l.next
		l.next.prev = l.prev
		delete(c.lruHash, key)
	}
}

func (c *simpleCache) Get(key string) (interface{}, bool) {
	c.Lock()
	defer c.Unlock()
	n := c.hash(key)
	p := c.hashtable[n]
	prev := p
	for p != nil {
		if p.key == key {
			// 过期删除
			if p.expired() {
				c.delLru(key)
				if c.hashtable[n] == p {
					c.hashtable[n] = p.next
				} else {
					prev.next = p.next
				}
				c.num--
				c.memory -= sizeof(p.value)
				return nil, false
			}
			c.lru(key)
			return p.value, true
		}
		prev = p
		p = p.next
	}
	return nil, false
}

func (c *simpleCache) Del(key string) bool {
	c.Lock()
	defer c.Unlock()
	return c.del(key)
}

func (c *simpleCache) del(key string) bool {
	n := c.hash(key)
	p := c.hashtable[n]
	prev := p
	for p != nil {
		if p.key == key {
			c.delLru(key)
			if c.hashtable[n] == p {
				c.hashtable[n] = p.next
			} else {
				prev.next = p.next
			}
			c.num--
			c.memory -= sizeof(p.value)
			return true
		}
		prev = p
		p = p.next
	}
	return false
}

func (c *simpleCache) Exists(key string) bool {
	_, ok := c.Get(key)
	return ok
}

func (c *simpleCache) Flush() bool {
	c.Lock()
	defer c.Unlock()
	c.num = 0
	c.memory = 0
	c.hashtable = make([]*entry, c.cap)
	c.lruhead = &lru{}
	c.lrutail = &lru{}
	c.lruHash = map[string]*lru{}
	c.lruhead.next = c.lrutail
	c.lrutail.prev = c.lruhead
	return true
}

func (c *simpleCache) Keys() int64 {
	return int64(c.num)
}
