package main

//数字表
type NumberTable struct {
	key int
	val int
}

//构建最小堆
func minHeap(heap []NumberTable, k int) {
	for i := k/2 - 1; i >= 0; i-- {
		if heap[i].val > heap[(i+1)*2-1].val {
			tmp := heap[i]
			heap[i] = heap[(i+1)*2-1]
			heap[(i+1)*2-1] = tmp
			//0123
			//1234
		}
		if k > (i+1)*2 && heap[i].val > heap[(i+1)*2].val {
			tmp := heap[i]
			heap[i] = heap[(i+1)*2]
			heap[(i+1)*2] = tmp
		}
	}
}

func topKFrequent(nums []int, k int) []int {
	//hashmap计算各个数字次数
	var numMap = make(map[int]int)
	for _, val := range nums {
		numMap[val]++
	}
	//构建优先队列
	var heap = make([]NumberTable, k)
	var i = 0
	for key, val := range numMap {
		if i >= k {
			break
		}
		heap[i] = NumberTable{key, val}
		i++
		numMap[key] = 0
	}
	//构建一次最小堆
	minHeap(heap, k)
	for key, val := range numMap {
		if val == 0 {
			continue
		}
		//后面的继续进入堆,对比第一个位置
		if heap[0].val < val {
			heap[0] = NumberTable{key, val}
			minHeap(heap, k)
		}
	}

	var ret []int
	for _, val := range heap {
		ret = append(ret, val.key)
	}
	return ret
}

func main() {
	topKFrequent([]int{5, -3, 9, 1, 7, 7, 9, 10, 2, 2, 10, 10, 3, -1, 3, 7, -9, -1, 3, 3}, 3)
}
