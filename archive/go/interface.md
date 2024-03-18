## GO Interface(接口)
> 记哪记得住啊,不如顺便水一点博客,忘记了还能过来翻翻

go的接口值是由两部分组成,被称为接口的动态类型和动态值
动态类型部分储存着数据的类型,动态值部分储存着值.

来点栗子
```go
type If interface {
	Demo() int
}
type S struct {
	a int
}
func (s *S) Demo() int {
	s.a++
	return s.a
}
```
上面是额外的定义

```go
func TestInterface(t *testing.T) {
	var i If
	s := S{}
	i = &s
	i.Demo()
	assert.Equal(t, s.a, 1)
}
```
值得一提的是第四行`i=&s`,这是由于声明S的Demo方法时决定的,接收器类型为`*S`,那么只有是*S类型才会有Demo方法.如果去掉`&`将会报错.

go中接口是可以判断相等的,相等的条件是,动态类型和动态值全部相等
```go
func TestInterfaceEqual(t *testing.T) {
	var i1, i2 If
	s := S{a: 2}
	assert.Equal(t, i1, i2)
	i1 = &s
	i2 = &s
	assert.Equal(t, i1, i2)
	var s2 *S
	i1 = s2
	i2 = nil
	t.Logf("s2:%v,i1 value:%v,i2 value:%v,i1 type:%T,i2 type:%T",
		s2, i1, i2, i1, i2)
	assert.NotEqual(t, i1, i2)
}
```
例如上面的例子,最后一个,虽然s2,i1,i2的值输出都是nil,但是i1,i2的type不相等,所以i1和i2是不相等的
