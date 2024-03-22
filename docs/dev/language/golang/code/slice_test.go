package code

import (
	"fmt"
	"github.com/stretchr/testify/assert"
	"reflect"
	"testing"
	"unsafe"
)

func TestSliceArray(t *testing.T) {
	var arr = [...]int{1, 2, 3}
	var slice = []int{1, 2, 3}
	ModifyArray(arr)
	ModifySlice(slice)
	t.Logf("%v %v\n", arr, slice)
	assert.NotEqual(t, arr[2], 1)
	assert.Equal(t, slice[2], 1)
}

func ModifyArray(arr [3]int) {
	fmt.Println(arr)
	arr[2] = 1
}

func ModifySlice(slice []int) {
	fmt.Println(slice)
	slice[2] = 1
}

func TestSlice(t *testing.T) {
	slice := []int{1, 2, 3}
	arr := [...]int{1, 2, 3}
	fmt.Printf("切片：底层数组指针：%d 切片：指针%v reflect.SliceHeader: %+v\n",
		unsafe.SliceData(slice), unsafe.Pointer(&slice), (*reflect.SliceHeader)(unsafe.Pointer(&slice)))
	SlicePointer(slice)
	fmt.Printf("数组：底层数组指针：%d 数组指针：%d\n", unsafe.Pointer(&arr), unsafe.Pointer(&arr[0]))
	ArrayPointer(arr)
}

func SlicePointer(slice []int) {
	fmt.Printf("切片：底层数组指针：%d 切片：指针%v reflect.SliceHeader: %+v\n",
		unsafe.SliceData(slice), unsafe.Pointer(&slice), (*reflect.SliceHeader)(unsafe.Pointer(&slice)))
}

func ArrayPointer(arr [3]int) {
	fmt.Printf("数组：底层数组指针：%d 数组指针：%d\n", unsafe.Pointer(&arr), unsafe.Pointer(&arr[0]))
}

func TestModify(t *testing.T) {
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

func TestModifyTwoSlice(t *testing.T) {
	arr := [...]int{1, 2, 3, 4, 5, 6, 7, 8, 9}
	slice1 := arr[1:5]
	slice2 := arr[3:8]
	slice1[2] = 8
	t.Logf("%v %v %v\n", arr, slice1, slice2)
	assert.Equal(t, slice1[2], slice2[0], arr[3])
}

func TestCap(t *testing.T) {
	slice := []int{1, 2, 3, 4}
	fmt.Printf("len:%d cap: %d\n", len(slice), cap(slice))
	slice = append(slice, make([]int, 11)...)
	fmt.Printf("len:%d cap: %d\n", len(slice), cap(slice)) // oldcap<256，直接使用新切片容量
	slice = append(slice, make([]int, 260)...)
	fmt.Printf("len:%d cap: %d\n", len(slice), cap(slice)) // oldcap<256，直接使用新切片容量
	slice = append(slice, make([]int, 250)...)
	fmt.Printf("len:%d cap: %d\n", len(slice), cap(slice)) // 1.25倍循环扩容
}

func TestString(t *testing.T) {
	str := "hello"
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	str = "world"
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	str = str[:3]
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
}

func TestStringAsSlice(t *testing.T) {
	str := "hello,world"
	hello := str[:5]
	world := str[6:]
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	fmt.Printf("%d %+v\n", &hello, (*reflect.StringHeader)(unsafe.Pointer(&hello)))
	fmt.Printf("%d %+v\n", &world, (*reflect.StringHeader)(unsafe.Pointer(&world)))
}

func TestCopy(t *testing.T) {
	str := "hello,world"
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	b := []byte(str)
	fmt.Printf("%d %+v\n", unsafe.Pointer(&b), (*reflect.SliceHeader)(unsafe.Pointer(&b)))
}

func TestZeroCopy(t *testing.T) {
	str := "hello,world"
	fmt.Printf("%d %+v\n", &str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	b := unsafe.Slice(unsafe.StringData(str), len(str))
	fmt.Printf("%s %+v\n", b, (*reflect.SliceHeader)(unsafe.Pointer(&b)))
}

func TestConvertZeroCopy(t *testing.T) {
	n := []int64{1, 2, 3, 4}
	// 注意精度问题
	fmt.Printf("%+v\n", (*reflect.SliceHeader)(unsafe.Pointer(&n)))
	f := unsafe.Slice(unsafe.SliceData(n), len(n))
	fmt.Printf("%v %+v\n", f, (*reflect.SliceHeader)(unsafe.Pointer(&f)))
}

func TestReplace(t *testing.T) {
	str := "hello,world"
	fmt.Printf("%v %+v\n", str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
	b := []byte(str)
	b[1] = 'a'
	str = unsafe.String(unsafe.SliceData(b), len(b))
	fmt.Printf("%v %+v\n", str, (*reflect.StringHeader)(unsafe.Pointer(&str)))
}
