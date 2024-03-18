package main

var mapping = [10][4]int{
	{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 1, 1, 0}, {1, 2, 0, 0}, {2, 0, 0, 0}, {2, 1, 0, 0}, {2, 1, 1, 0}, {2, 1, 1, 1}, {1, 3, 0, 0},
}

var roman = []string{
	"I", "V", "X", "L", "C", "D", "M",
}

func intToRoman(num int) string {
	var ret string
	bit := 0
	for num > 0 {
		t := num % 10
		num /= 10
		tmp := ""
		for _, val := range mapping[t] {
			if val == 0 {
				break
			}
			tmp += roman[val+(bit*2)-1]
		}
		bit++
		ret = tmp + ret
	}
	return ret
}

func main() {
	println(intToRoman(1994))
}

//dalao
// func intToRoman(num int) string {
// 	var romanInt = []int{1000, 900, 500 ,400, 100, 90, 50, 40, 10, 9, 5, 4, 1}
// 	var romanStr = []string{"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}
// 	var res = ""
// 	for i, n := range romanInt {
// 		for num >= n {
// 			res += romanStr[i]
// 			num -= n
// 		}
// 	}
// 	return res
// }
