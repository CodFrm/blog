package main

import (
	"fmt"
	"sort"
)

func triangleNumber(nums []int) int {
	sort.Sort(sort.IntSlice(nums))
	ret := 0
	for i := range nums {
		i2 := len(nums) - 2
		if i2 > i {
			num := nums[i] + nums[i2]
			for i3 := len(nums) - 1; i3 > i2; i3-- {
				if num > nums[i3] {
					ret += (i3 - i2) + (i2 - i - 1)
					break
				}
			}
		}

	}
	return ret
}

func main() {
	fmt.Println(triangleNumber([]int{2, 3, 4, 4})) //4

	fmt.Println(triangleNumber([]int{2, 2, 3, 4})) //3
}
