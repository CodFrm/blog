package main

import "fmt"

type IA interface {
	FuncA()
	FuncB()
}

type A struct {
	IA
}

func (a *A) FuncA() {
	fmt.Println("class a,func a")
}

type B struct {
}

func (b *B) FuncA() {
	fmt.Println("class b,func a")
}

func (b *B) FuncB() {
	fmt.Println("class b,func b")
}

func main() {
	var a IA = new(A)
	a.FuncA()
	// a.FuncB() //报错的
	var a2 IA = &A{IA: new(B)}
	a2.FuncA()
	a2.FuncB()
}
