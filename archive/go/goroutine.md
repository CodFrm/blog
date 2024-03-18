## Go Goroutine(协程)
> 原来对协程有一些了解,用C实现过一个辣鸡的协程.通过上下文的切换,将io堵塞的时间切换成到其它的协程继续运行,不让cpu歇息一下

go中运行协程通过`go`关键词就可以运行一个协程,非常简单

来看一个例子:
```go
func main() {
	var tmp string
	for i := 0; i < 10; i++ {
		go func() {
			for {
			}
		}()
	}
	fmt.Scanln(&tmp)
}
```
可以直接跑满CPU(我的电脑是4核8线程)

然后我改改
```go
func main() {
	var tmp string
	for i := 0; i < 10; i++ {
		go func(i int) {
			for {
				fmt.Println(i)
			}
		}(i)
	}
	fmt.Scanln(&tmp)
}
```
我电脑的CPU降到了40%,这是因为当所有的协程都堵塞住了(打印数据),给了CPU喘息的机会,而且因为达到了输出的最大限制,增加协程也不会再增加CPU了(io瓶颈)

再来一个例子,这里用了`runtime.GOMAXPROCS(1)`这个函数,它可以设置我们的协程运行指定个数的CPU核里面,这里只用了一个核
```go
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
```
然后CPU维持再18%左右,总CPU在20%左右,我想是另外的线程占用了剩余的CPU.(然后还有超频?反正我是猜的)

在go1.14实现了抢占式的协程，上述协程可能会无法再往下执行

