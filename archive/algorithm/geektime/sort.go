package geektime

func bubble_1(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	for i := range ret {
		for j := range ret[i:] {
			j = i + j
			if ret[i] > ret[j] {
				ret[i], ret[j] = ret[j], ret[i]
			}
		}
	}
	return ret
}

func bubble_2(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	for i := range ret {
		flag := false
		for j := range ret[i:] {
			j = i + j
			if ret[i] > ret[j] {
				flag = true
				ret[i], ret[j] = ret[j], ret[i]
			}
		}
		if !flag {
			break
		}
	}
	return ret
}

func insert(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	for i := 1; i < len(ret); i++ {
		val := ret[i]
		j := i - 1
		for ; j >= 0; j-- {
			if ret[j] > val {
				ret[j+1] = ret[j]
			} else {
				break
			}
		}
		ret[j+1] = val
	}
	return ret
}

func selection(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	for i := range ret {
		min := i
		for j := range ret[i:] {
			j = i + j
			if ret[min] > ret[j] {
				min = j
			}
		}
		ret[min], ret[i] = ret[i], ret[min]
	}
	return ret
}

//归并排序
func merge_sort(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	dichotomize(ret, 0, len(ret)-1)
	return ret
}

func dichotomize(arr []int, i, j int) (int, int) {
	if i >= j {
		return i, j
	}
	i1, j1 := dichotomize(arr, i, (j-i)/2+i)
	i2, j2 := dichotomize(arr, (j-i)/2+i+1, j)
	merge(arr, i1, j1, i2, j2)
	return i, j
}

func merge(arr []int, i1, j1, i2, j2 int) {
	pos2 := i2
	tmp := make([]int, j2-i1+1)
	tmppos := 0
	pos1 := i1
	for ; pos1 <= j1 && pos2 <= j2; tmppos++ {
		if arr[pos1] > arr[pos2] {
			tmp[tmppos] = arr[pos2]
			pos2++
		} else {
			tmp[tmppos] = arr[pos1]
			pos1++
		}
	}
	//剩余
	if pos1 <= j1 {
		for ; pos1 <= j1; pos1++ {
			tmp[tmppos] = arr[pos1]
			tmppos++
		}
	} else {
		for ; pos2 <= j2; pos2++ {
			tmp[tmppos] = arr[pos2]
			tmppos++
		}
	}
	//copy
	tmppos = 0
	for i := i1; i <= j2; i++ {
		arr[i] = tmp[tmppos]
		tmppos++
	}
}

func qsort(arr []int) []int {
	ret := make([]int, len(arr))
	copy(ret, arr)
	return qsort_c(ret)
}

func qsort_c(arr []int) []int {
	n := len(arr)
	if n == 2 {
		if arr[0] > arr[1] {
			arr[0], arr[1] = arr[1], arr[0]
			return arr
		}
		return arr
	} else if n <= 1 {
		return arr
	}
	val := arr[n/2]
	arr[0], arr[n/2] = arr[n/2], arr[0]
	j := 1
	for i := 1; i < n; i++ {
		if arr[i] < val {
			arr[i], arr[j] = arr[j], arr[i]
			j++
		}
	}
	qsort_c(arr[:j])
	qsort_c(arr[j:])
	return arr
}
