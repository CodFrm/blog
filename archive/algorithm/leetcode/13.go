package main

var mapping = map[string]int{
	"I": 1,
	"V": 5,
	"X": 10,
	"L": 50,
	"C": 100,
	"D": 500,
	"M": 1000,
}

func romanToInt(s string) int {
	var ret, last int
	for _, chr := range s {
		val := mapping[string(chr)]
		if last < val {
			ret = val + ret - (last * 2)
		} else {
			ret += val
		}
		last = val
	}
	return ret
}

func main() {
	println(romanToInt("MCMXCIV")) //5
	println(romanToInt("IIIV"))    //4
}
