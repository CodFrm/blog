package main

func main() {
	cache := NewCache()
	cache.SetMaxMemory("100MB")
	cache.Set("int", 1, 0)
	cache.Set("bool", false, 0)
	cache.Set("data", map[string]interface{}{"a": 1}, 0)
	cache.Get("int")
	cache.Del("int")
	cache.Flush()
	cache.Keys()
}
