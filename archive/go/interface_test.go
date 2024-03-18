package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

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
func TestInterface(t *testing.T) {
	s := S{}
	var i If
	i = &s
	i.Demo()
	assert.Equal(t, s.a, 1)
}

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
