package geektime

import (
	"github.com/stretchr/testify/assert"
	"math/rand"
	"testing"
)

var sortArr100, sortArr1000 []int

func TestMain(m *testing.M) {
	sortArr100 = randIntSlice(100)
	sortArr1000 = randIntSlice(1000)
	m.Run()
}

func Test_bubble(t *testing.T) {
	arr := []int{5, 3, 4, 1, 2}
	ret := bubble_1(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5}, ret)
	ret = bubble_2(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5}, ret)
	arr = randIntSlice(10)
	ret = bubble_1(arr)
	ret2 := bubble_2(arr)
	assert.Equal(t, ret2, ret)
}

func randIntSlice(n int) []int {
	ret := make([]int, n)
	for i := 0; i < n; i++ {
		ret[i] = rand.Int()
	}
	return ret
}

func BenchmarkBubble1100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		bubble_1(arr)
	}
}

func BenchmarkBubble11000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		bubble_1(arr)
	}
}

func BenchmarkBubble2100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		bubble_2(arr)
	}
}

func BenchmarkBubble21000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		bubble_2(arr)
	}
}

func Test_insert(t *testing.T) {
	arr := []int{5, 3, 4, 1, 2}
	ret := insert(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5}, ret)
	arr = randIntSlice(10)
	ret = bubble_1(arr)
	ret2 := insert(arr)
	assert.Equal(t, ret, ret2)
}

func BenchmarkInsert100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		insert(arr)
	}
}

func BenchmarkInsert1000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		insert(arr)
	}
}

func Test_selection(t *testing.T) {
	arr := []int{5, 3, 4, 1, 2}
	ret := selection(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5}, ret)
	arr = randIntSlice(10)
	ret = bubble_1(arr)
	ret2 := selection(arr)
	assert.Equal(t, ret, ret2)
}

func BenchmarkSelect100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		selection(arr)
	}
}

func BenchmarkSelect1000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		selection(arr)
	}
}

func Test_merge_sort(t *testing.T) {
	arr := []int{5, 3, 4, 1, 2, 6}
	ret := merge_sort(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5, 6}, ret)
	arr = randIntSlice(10)
	ret = bubble_1(arr)
	ret2 := merge_sort(arr)
	assert.Equal(t, ret, ret2)
}

func BenchmarkMerge100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		merge_sort(arr)
	}
}

func BenchmarkMerge1000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		merge_sort(arr)
	}
}

func Test_qsort(t *testing.T) {
	arr := []int{5, 3, 4, 1, 2, 6}
	ret := qsort(arr)
	assert.Equal(t, []int{1, 2, 3, 4, 5, 6}, ret)
	arr = randIntSlice(10)
	ret = bubble_1(arr)
	ret2 := qsort(arr)
	assert.Equal(t, ret, ret2)
}

func BenchmarkQuickSort100(b *testing.B) {
	arr := sortArr100
	for i := 0; i < b.N; i++ {
		qsort(arr)
	}
}

func BenchmarkQuickSort1000(b *testing.B) {
	arr := sortArr1000
	for i := 0; i < b.N; i++ {
		qsort(arr)
	}
}
