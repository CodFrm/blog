package main

import (
	"strconv"
	"testing"
	"time"

	"github.com/allegro/bigcache/v3"
)

func BenchmarkMapSet(b *testing.B) {
	m := make(map[string]int, b.N)
	for i := 0; i < b.N; i++ {
		m[strconv.Itoa(i)] = i
	}
}

func BenchmarkMapGetSet(b *testing.B) {
	m := make(map[string]int, b.N)
	for i := 0; i < 1000; i++ {
		m[strconv.Itoa(i)] = i
	}
	for i := 0; i < b.N; i++ {
		_, _ = m[strconv.Itoa(i%1000)]
	}
}

func BenchmarkCacheSet(b *testing.B) {
	cache := NewCache()
	for i := 0; i < b.N; i++ {
		cache.Set(strconv.Itoa(i), i, 0)
	}
}

func BenchmarkCacheSetGet(b *testing.B) {
	cache := NewCache()
	for i := 0; i < 1000; i++ {
		cache.Set(strconv.Itoa(i), i, 0)
	}
	for i := 0; i < b.N; i++ {
		cache.Get(strconv.Itoa(i % 1000))
	}
}

func BenchmarkBigCacheSet(b *testing.B) {
	cache, _ := bigcache.NewBigCache(bigcache.DefaultConfig(10 * time.Minute))
	for i := 0; i < b.N; i++ {
		cache.Set(strconv.Itoa(i), []byte(strconv.Itoa(i)))
	}
}

func BenchmarkBigCacheSetGet(b *testing.B) {
	cache, _ := bigcache.NewBigCache(bigcache.DefaultConfig(10 * time.Minute))
	for i := 0; i < 1000; i++ {
		cache.Set(strconv.Itoa(i), []byte(strconv.Itoa(i)))
	}
	for i := 0; i < b.N; i++ {
		cache.Get(strconv.Itoa(i % 1000))
	}
}
