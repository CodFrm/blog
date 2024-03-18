package main

import "log"

//https://leetcode-cn.com/problems/lru-cache/
//数组形式实现(O(n)),另外可以用双向链表+哈希表实现(O(1))
type LRUCache struct {
	capacity int
	cache    map[int]int
	lru      []int
	head     int
	tail     int
}

func Constructor(capacity int) LRUCache {
	return LRUCache{
		capacity: capacity,
		cache:    make(map[int]int),
		lru:      make([]int, capacity),
		head:     0, tail: 0,
	}
}

func (this *LRUCache) Get(key int) int {
	v, ok := this.cache[key]
	if ok {
		this.FloatUp(key)
		return v
	}
	return -1
}

func (this *LRUCache) Put(key int, value int) {
	this.cache[key] = value
	this.FloatUp(key)
}

func (this *LRUCache) FloatUp(key int) {
	//查找位
	for i := 0; true; i++ {
		var pos = this.pos(i + this.tail)
		if key == this.lru[pos] {
			//提到第一位,前面的后移
			for ; true; i++ {
				var pos = this.pos(i + this.tail)
				if pos == this.head {
					//到头了,跳出
					break
				}
				this.lru[pos] = this.lru[this.pos(pos+1)]
			}
			this.head = this.head - 1
			if this.head == -1 {
				this.head = this.capacity - 1
			}
			break
		} else if pos == this.head {
			//到头了,跳出
			break
		}
	}
	var tmp = this.lru[this.tail]
	this.head = this.pos(this.head + 1)
	this.lru[this.head] = key
	//判断是否已满
	if this.head == this.tail {
		//满,删除最后一位
		delete(this.cache, tmp)
		this.tail = this.pos(this.tail + 1)
	}
}

func (this *LRUCache) pos(pos int) int {
	return pos % this.capacity
}

/**
 * Your LRUCache object will be instantiated and called as such:
 * obj := Constructor(capacity);
 * param_1 := obj.Get(key);
 * obj.Put(key,value);
 */

func main() {
	cache := Constructor(3)
	cache.Put(1, 1)
	cache.Put(2, 2)
	cache.Put(3, 3)
	cache.Put(4, 4)
	cache.Get(4)
	cache.Get(3)
	cache.Get(2)
	cache.Get(1)
	cache.Put(5, 5)
	cache.Get(1)
	cache.Get(2)
	cache.Get(3)
	cache.Get(4)
	cache.Get(5)
}

func Equal(expected, actual int) {
	if expected != actual {
		log.Fatalln(expected, "!=", actual)
	} else {
		log.Println("OK")
	}
}
