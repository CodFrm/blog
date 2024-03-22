---
title: Go Slice(切片)
---

> 感觉切片只要知道底层是引用的一个数组对象,就挺好理解了.这里写下一些笔记,方便记忆和以后再来查找.

### 切片和数组

切片由三个部分组成:指针(指向底层数组),长度(当前切片使用的长度),容量(切片能包含多少个成员)

然后还有一句和数组相关的:当调用一个函数的时候,函数的每个调用参数将会被赋值给函数内部的参数变量,所以函数参数变量接收的是一个复制的副本,并不是原始调用的变量。(所以数组作为参数,是低效的,还需要进行一次数组的拷贝,可以使用数组指针)

然后如果我们想要传递给一个函数一个数组,函数需要对数组进行修改,我们必须使用数组指针(用 return 当然也不是不行啦?)

但是切片就不需要,来个例子:

```go
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
	println(arr)
	arr[2] = 1
}
func ModifySlice(slice []int) {
	println(slice)
	slice[2] = 1
}
```

凭啥切片就行,大家明明都长得都差不多.
前面说了,切片是由三个部分组成,然后数组传入的是数组的副本.其实我觉得 go 里所有的类型传入的都是对应的副本,切片也是,指针也是的(值传递).

那都是副本拷贝,那么咋切片可以修改?
`切片是由:数组指针,长度,容量组成的`,来划一下重点.
副本传的也是上面这些东西.然后修改切片的时候呢,实际上是通过切片里面的数组指针去修改了,并没有修改切片的值(数组指针,长度,容量).

等看完下面再写另外一个情况,在函数里面,给切片增加成员,会怎么样?

### 切片

定义一个数组和定义一个切片的区别是[...]和\[\](当然还有其他的定义方式)

```go
func Test_DefineSlice(t *testing.T) {
	var arr = [...]int{1, 2, 3}
	var slice1 = []int{1, 2, 3}
	var slice2 = make([]int, 3)
	var slice3 = arr[:]
	fmt.Printf("arr type=%v len=%d cap=%d\n", reflect.TypeOf(arr).String(), len(arr), cap(arr))
	fmt.Printf("slice1 type=%v len=%d cap=%d\n", reflect.TypeOf(slice1).String(), len(slice1), cap(slice1))
	fmt.Printf("slice2 type=%v len=%d cap=%d\n", reflect.TypeOf(slice2).String(), len(slice2), cap(slice2))
	fmt.Printf("slice3 type=%v len=%d cap=%d\n", reflect.TypeOf(slice3).String(), len(slice3), cap(slice3))
}
//Result:
//arr type=[3]int len=3 cap=3
//slice1 type=[]int len=3 cap=3
//slice2 type=[]int len=3 cap=3
//slice3 type=[]int len=3 cap=3
```

上面方法中的切片是会自动创建一个底层数组,如果切片直接引用一个创建好了的数组呢?
我的猜想是在切片里面修改值,原数组也会跟着一起变(切片指针指向的就是这一个数组)
然后我想再验证一下,如果我的切片再增加一个成员(超出数组限制),那么还会变化吗?
我的猜想是会重新分配到另外一个数组去,然后导致引用的数组不会发生改变(切片指针指向的已经是另外一个数组了)

```go
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
```

验证通过^\_^

再来试试两个切片共享一个数组

```go
func Test_ModifyTwoSlice(t *testing.T) {
	arr := [...]int{1, 2, 3, 4, 5, 6, 7, 8, 9}
	slice1 := arr[1:5]
	slice2 := arr[3:8]
	slice1[2] = 8
	t.Logf("%v %v %v\n", arr, slice1, slice2)
	assert.Equal(t, slice1[2], slice2[0], arr[3])
}
```

一样的全部一起修改成功了

### append

然后我们来看看 append

```go
func Test_Append(t *testing.T) {
	slice := []int{1, 2, 3}
	println(slice)
	slice = append(slice, 1)
	println(slice)
	slice = append(slice, 1)
	println(slice)
}
// Result:
// slice type=[]int len=3 cap=3
// [3/3]0xc00005e3e0
// slice type=[]int len=4 cap=6
// [4/6]0xc00008c060
// slice type=[]int len=5 cap=6
// [5/6]0xc00008c060
```

当容量够的时候切片的内存地址没有发生变化,不够的时候进行了扩容,地址改变了.

刚刚写的时候发现了一个问题,就是每次扩容的大小,我写了一个循环来展示

```go
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
```

好吧,我以为扩容的容量,~~如果不是 make 的话是按照前一次容量的两倍来扩容的,是 make 就是每次增加的容量是固定的,事实证明我想多了~~

### End

再回到最开始,在函数里面,增加切片的成员.我想应该有了答案.

```go
func ModifySlice(slice []int) {
	slice[2] = 1
	slice = append(slice, 4)
	slice[2] = 3
}
```

我把之前的`ModifySlice`方法修改了一下,然后成员没加,后面再修改回去为 3 也没有发生变化了.
这是因为 append 的时候因为容量不够扩容了,导致底层数组指针发生了改变,但是传进来的切片是外面切片的副本,修改这个切片里面的数组指针不会影响到外面的切片

#### 奇淫巧技

例如这个(go 圣经里面抄的 233),reverse 的参数是切片,但是我们需要处理的是一个数组,这里我们就可以直接用`arr[:]`把数组转化为切片进行处理

```go
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
```

> 暂时只想到这些,再有的话,以后继续补.如有错误希望指教.
