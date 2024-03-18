package main

import (
	"fmt"
	"runtime"
)

func main() {
	var tmp string
	runtime.GOMAXPROCS(1)
	for i := 0; i < 10; i++ {
		go func() {
			for {
			}
		}()
	}
	fmt.Scanln(&tmp)
}
