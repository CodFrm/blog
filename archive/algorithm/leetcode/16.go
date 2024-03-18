package main

import (
	"sort"
)

func threeSumClosest(nums []int, target int) int {
	min := int(^uint(0) >> 1)
	minAbs := min
	numLen := len(nums)
	sort.Slice(nums, func(i, j int) bool {
		return nums[i] < nums[j]
	})
	for i := 0; i < numLen-2; i++ {
		left, right := i+1, numLen-1
		for left < right {
			tmp := nums[i] + nums[left] + nums[right]
			tmpAbs := target - tmp
			if tmpAbs < 0 {
				tmpAbs = -tmpAbs
			}
			if tmp < target {
				left++
			} else {
				right--
			}
			if tmpAbs < minAbs {
				min = tmp
				minAbs = tmpAbs
			}
		}
	}
	return min
}

func main() {
	println(threeSumClosest([]int{1, 2, 4, 8, 16, 32, 64, 128}, 82))
}

// [1,2,3,4,5,6,7,8,9]
// [-1，2，1，-4]
