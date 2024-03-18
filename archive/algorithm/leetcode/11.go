package main

func maxArea(height []int) int {
	var left, right, max = 0, len(height) - 1, 0
	for left < right {
		area := 0
		if height[left] > height[right] {
			area = height[right] * (right - left)
			right--
		} else {
			area = height[left] * (right - left)
			left++
		}
		if area > max {
			max = area
		}
	}
	return max
}

func main() {
	println(maxArea([]int{1, 2, 54, 75, 3, 5, 7, 93, 3}))
}
