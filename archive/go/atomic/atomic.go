package main

import (
	"sync"
	"sync/atomic"
	"time"
)

func main() {
	t := time.Now()
	sum := int64(0)
	for i := int64(0); i < 500000000; i++ {
		sum += i
	}
	t1 := time.Now().Sub(t)
	println(sum)
	println(t1.String())
	//锁
	t = time.Now()
	sum = 0
	var mutex sync.Mutex
	for i := int64(0); i < 500000000; i++ {
		mutex.Lock()
		sum += i
		mutex.Unlock()
	}
	t1 = time.Now().Sub(t)
	println(sum)
	println(t1.String())
	//原子操作
	t = time.Now()
	sum = 0
	for i := int64(0); i < 500000000; i++ {
		atomic.AddInt64(&sum, i)
	}
	t1 = time.Now().Sub(t)
	println(sum)
	println(t1.String())
}
