package main

import (
	"fmt"
	"reflect"
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_Func(t *testing.T) {
	var arr = [...]int{1, 2, 3}
	var slice = []int{1, 2, 3}
	ModifyArray(arr)
	ModifySlice(slice)
	t.Logf("%v %v\n", arr, slice)
	assert.NotEqual(t, arr[2], 1)
	assert.Equal(t, slice[2], 1)
}

func ModifyArray(arr [3]int) {
	arr[2] = 1
}

func ModifySlice(slice []int) {
	slice[2] = 1
	slice = append(slice, 4)
	slice[2] = 3
}

func Test_DefineSlice(t *testing.T) {
	var arr = [...]int{1, 2, 3}
	var slice1 = []int{1, 2, 3}
	var slice2 = make([]int, 3)
	var slice3 = arr[:]
	t.Logf("arr type=%v len=%d cap=%d\n", reflect.TypeOf(arr).String(), len(arr), cap(arr))
	t.Logf("slice1 type=%v len=%d cap=%d\n", reflect.TypeOf(slice1).String(), len(slice1), cap(slice1))
	t.Logf("slice2 type=%v len=%d cap=%d\n", reflect.TypeOf(slice2).String(), len(slice2), cap(slice2))
	t.Logf("slice3 type=%v len=%d cap=%d\n", reflect.TypeOf(slice3).String(), len(slice3), cap(slice3))
}

func Test_Modify(t *testing.T) {
	arr := [...]int{1, 2, 3, 4, 5, 6, 7}
	slice := arr[:]
	slice[4] = 8
	t.Logf("arr[4]=%v,slice[4]=%v\n", arr[4], slice[4])
	assert.Equal(t, slice[4], arr[4])
	slice = append(slice, 9)
	slice[5] = 10
	t.Logf("arr[4]=%v,slice[4]=%v\n", arr[4], slice[4])
	assert.Equal(t, slice[4], arr[4])
	t.Logf("arr[5]=%v,slice[5]=%v\n", arr[5], slice[5])
	assert.NotEqual(t, slice[5], arr[5])
}

func Test_ModifyTwoSlice(t *testing.T) {
	arr := [...]int{1, 2, 3, 4, 5, 6, 7, 8, 9}
	slice1 := arr[1:5]
	slice2 := arr[3:8]
	slice1[2] = 8
	t.Logf("%v %v %v\n", arr, slice1, slice2)
	assert.Equal(t, slice1[2], slice2[0], arr[3])
}

func Test_Append(t *testing.T) {
	slice := []int{1, 2, 3}
	fmt.Printf("slice type=%v len=%d cap=%d\n", reflect.TypeOf(slice).String(), len(slice), cap(slice))
	println(slice)
	slice = append(slice, 1)
	fmt.Printf("slice type=%v len=%d cap=%d\n", reflect.TypeOf(slice).String(), len(slice), cap(slice))
	println(slice)
	slice = append(slice, 1)
	fmt.Printf("slice type=%v len=%d cap=%d\n", reflect.TypeOf(slice).String(), len(slice), cap(slice))
	println(slice)
}

func Test_Cap(t *testing.T) {
	slice1 := []int{1, 2, 3}
	slice2 := make([]int, 3, 3)
	last := [2]int{0, 0}
	for i := 0; i < 100; i++ {
		slice1 = append(slice1, 1)
		slice2 = append(slice2, 1)
		if last[0] != cap(slice1) {
			println(slice1)
			last[0] = cap(slice1)
		}
		if last[1] != cap(slice2) {
			println(slice2)
			last[1] = cap(slice2)
		}
	}
}

func Test_Demo(t *testing.T) {
	arr := [...]int{1, 2, 3, 4}
	reverse(arr[:])
	t.Logf("%v\n", arr)
}

func reverse(s []int) {
	for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
		s[i], s[j] = s[j], s[i]
	}
}
