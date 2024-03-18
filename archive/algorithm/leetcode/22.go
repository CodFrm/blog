package main

func dfs(str string, ret *[]string, n, l, r int) {
	if r >= n {
		*ret = append(*ret, str)
		return
	}
	if l < n {
		dfs(str+"(", ret, n, l+1, r)
	}
	if r < l {
		dfs(str+")", ret, n, l, r+1)
	}
}

func generateParenthesis(n int) []string {
	ret := []string{}
	dfs("", &ret, n, 0, 0)
	return ret
}

func main() {
	ret := generateParenthesis(3)
	for _, val := range ret {
		println(val + " ")
	}
}
