package main

import "reflect"

// 简单的计算了,对于指针、切片等有问题
func sizeof(v interface{}) int {
	return int(reflect.TypeOf(v).Size())
}
