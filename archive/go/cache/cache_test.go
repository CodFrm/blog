package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"testing"
	"time"
)
import "github.com/stretchr/testify/assert"

func Test_cache_SetMaxMemory(t *testing.T) {
	tests := []struct {
		name      string
		args      string
		maxMemory int64
		want      bool
	}{
		{"case1", "1KB", 1024, true},
		{"case2", "1MB", 1024 * 1024, true},
		{"case3", "1GB", 1024 * 1024 * 1024, true},
		{"case4", "10G", 0, false},
		{"case5", "0", 0, false},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			c := &cache{}
			if got := c.SetMaxMemory(tt.args); got != tt.want || c.maxMemory != tt.maxMemory {
				t.Errorf("SetMaxMemory() = %v,maxMemory = %v, want %v,%v", got, c.maxMemory, tt.want, tt.maxMemory)
			}
		})
	}
}

func TestCache(t *testing.T) {
	cache := newCache(1024)
	cache.SetMaxMemory("100MB")
	assert.Equal(t, int64(1024*1024*100), cache.maxMemory)
	cache.Set("int", 1, 0)
	assert.Equal(t, int64(8), cache.memory)
	v, ok := cache.Get("int")
	assert.True(t, ok)
	assert.Equal(t, 1, v)
	cache.Del("int")
	_, ok = cache.Get("int")
	assert.False(t, ok)

	cache.Set("str", "ok", time.Microsecond)
	v, ok = cache.Get("str")
	assert.True(t, ok)
	assert.Equal(t, "ok", v)
	assert.Equal(t, int64(1), cache.Keys())
	time.Sleep(time.Microsecond)
	assert.Equal(t, int64(1), cache.Keys())
	assert.Equal(t, int64(16), cache.memory)

	v, ok = cache.Get("str")
	assert.False(t, ok)
	assert.Equal(t, int64(0), cache.Keys())
	assert.Equal(t, int64(0), cache.memory)

	cache.Flush()
	cache.Keys()
}

func TestClean(t *testing.T) {
	cache := newCache(1024)
	cache.SetMaxMemory("1KB")
	cache.Set("test1", 1, 0)
	cache.Set("test2", 10, 0)
	cache.Set("test3", 100, 0)
	cache.Set("test4", 1000, 0)

	time.Sleep(time.Microsecond)
	cache.Set("bigkey", [1024]int{0}, 0)
	assert.Equal(t, int64(1), cache.Keys())

	_, ok := cache.Get("test1")
	assert.False(t, ok)
	v, ok := cache.Get("bigkey")
	assert.Equal(t, [1024]int{0}, v)
	assert.True(t, ok)

	time.Sleep(time.Microsecond)
	cache.Set("test1", 1, 0)
	cache.Set("test2", 10, 0)
	cache.Set("test3", 100, 0)
	assert.Equal(t, int64(3), cache.Keys())

	v, ok = cache.Get("test3")
	assert.True(t, ok)
	assert.Equal(t, 100, v)
	v, ok = cache.Get("bigkey")
	assert.False(t, ok)
	assert.Nil(t, v)

}

// 测试扩容
func TestExpansion(t *testing.T) {
	cache := newCache(1024)
	cache.SetMaxMemory("1GB")
	for i := 0; i < 10000; i++ {
		cache.Set(fmt.Sprintf("%d", i), i, 0)
	}
	for i := 0; i < 10000; i++ {
		v, ok := cache.Get(fmt.Sprintf("%d", i))
		assert.True(t, ok)
		assert.Equal(t, i, v)
	}
	assert.Equal(t, int64(80000), cache.memory)
	assert.Equal(t, int64(10000), cache.num)
	cache.Flush()
	assert.Equal(t, int64(0), cache.memory)
	assert.Equal(t, int64(0), cache.num)
}

// 并发读写测试
func TestSetGet(t *testing.T) {
	var rn int64
	maxNum := int64(100000)
	cache := newCache(1024)
	cache.SetMaxMemory("1GB")
	var n int64 = -1
	wg := sync.WaitGroup{}
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for {
				i := atomic.AddInt64(&n, 1)
				if i > maxNum {
					break
				}
				cache.Set(fmt.Sprintf("%d", i), i, 0)
			}
		}()
	}
	wg.Wait()
	n = -1
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for {
				i := atomic.AddInt64(&n, 1)
				if i > maxNum {
					break
				}
				v, ok := cache.Get(fmt.Sprintf("%d", i))
				assert.True(t, ok)
				assert.Equal(t, i, v)
				atomic.AddInt64(&rn, 1)
			}
		}()
	}
	wg.Wait()
	assert.Equal(t, maxNum+1, rn)
	rn = 0
	cache.Flush()
	assert.Equal(t, int64(0), cache.memory)
	assert.Equal(t, int64(0), cache.num)
	// Flush 不会缩容,另外新建一个测试
	cache = newCache(1024)
	cache.SetMaxMemory("1GB")
	// 一边写 一边读
	c := make(chan int64, 100)
	n = -1
	wwg := sync.WaitGroup{}
	for i := 0; i < 100; i++ {
		wwg.Add(1)
		go func() {
			defer wwg.Done()
			for {
				i := atomic.AddInt64(&n, 1)
				if i > maxNum {
					break
				}
				cache.Set(fmt.Sprintf("%d", i), i, 0)
				c <- i
			}
		}()
	}
	go func() {
		wwg.Wait()
		close(c)
	}()
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for {
				i, ok := <-c
				if !ok {
					break
				}
				v, ok := cache.Get(fmt.Sprintf("%d", i))
				assert.True(t, ok)
				atomic.AddInt64(&rn, 1)
				assert.Equal(t, i, v)
			}
		}()
	}
	wg.Wait()
	assert.Equal(t, maxNum+1, rn)
}

// 测试清理
func TestLru(t *testing.T) {
	cache := newCache(2048)
	cache.SetMaxMemory("1KB")
	for i := 0; i < 100; i++ {
		cache.Set(fmt.Sprintf("%d", i), [...]int{1, 2, 3, 4, 5, 6, 7, 8}, 0)
		assert.True(t, cache.memory <= cache.maxMemory)
	}

}
