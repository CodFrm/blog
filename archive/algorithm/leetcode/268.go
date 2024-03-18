package main

func missingNumber(nums []int) int {
	ret := 0
	len := len(nums)
	for i := 0; i < len; i++ {
		ret = ret ^ i ^ nums[i]
	}
	return ret ^ len
}

func main() {
	println(missingNumber([]int{9, 6, 4, 2, 3, 5, 7, 0, 1}))
}
