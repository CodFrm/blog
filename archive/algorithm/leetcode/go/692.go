package main

//一模一样的题目
//347 刷一下

//数字表
type StringTable struct {
	key string
	val int
}

//构建最小堆
func minHeap(heap []StringTable, k int) {
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

func topKFrequent(words []string, k int) []string {
	//hashmap计算各个数字次数
	var numMap = make(map[string]int)
	for _, val := range words {
		numMap[val]++
	}
	//构建优先队列
	var heap = make([]StringTable, k)
	var i = 0
	for key, val := range numMap {
		if i >= k {
			break
		}
		heap[i] = StringTable{key, val}
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
			heap[0] = StringTable{key, val}
			minHeap(heap, k)
		}
	}
	//比那个多了一个排序,来次快排
	//TODO:快排
	var ret []string
	for i = k - 1; i >= 0; i-- {
		ret = append(ret, heap[i].key)
	}
	return ret
}

func main() {
	topKFrequent([]string{"the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is"}, 4)
}
