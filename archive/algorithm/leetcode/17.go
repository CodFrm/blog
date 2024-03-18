package main

var mapping = map[rune][]rune{
	'2': {'a', 'b', 'c'}, '3': {'d', 'e', 'f'},
	'4': {'g', 'h', 'i'}, '5': {'j', 'k', 'l'}, '6': {'m', 'n', 'o'},
	'7': {'p', 'q', 'r', 's'}, '8': {'t', 'u', 'v'}, '9': {'w', 'x', 'y', 'z'},
}

func dfs(digits string, ret []string) []string {
	if digits == "" {
		return ret
	}
	chars := mapping[rune(digits[0])]
	if len(ret) == 0 {
		for _, val := range chars {
			ret = append(ret, string(val))
		}
	} else {
		retLen := len(ret)
		for key, _ := range ret {
			for _, val := range chars {
				ret = append(ret, ret[key]+string(val))
			}
		}
		ret = ret[retLen:]
	}
	ret = dfs(digits[1:], ret)
	return ret
}

func letterCombinations(digits string) []string {
	return dfs(digits, []string{})
}

func main() {
	for _, val := range letterCombinations("26") {
		println(val)
	}
}
