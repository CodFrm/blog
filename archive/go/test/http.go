package main

import (
	"net/http"
	"time"
)

// 测试http是否会处理完请求后才结束
// 结果,不会

func main() {

	http.HandleFunc("/test", func(writer http.ResponseWriter, request *http.Request) {
		time.Sleep(time.Second * 5)
		writer.Write([]byte("success"))
	})

	http.ListenAndServe(":8080", nil)

}
