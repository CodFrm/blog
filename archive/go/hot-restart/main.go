package main

import (
	"context"
	"flag"
	"net"
	"net/http"
	"os"
	"os/exec"
	"os/signal"
	"sync"
	"syscall"
	"time"
)

// 一个停止的信号量
var (
	isStop   bool
	lock     sync.WaitGroup
	server   *http.Server
	listener net.Listener
	isReload bool
)

func init() {
	flag.BoolVar(&isReload, "reload", false, "")
	flag.Parse()
}

// 优雅停止中间件
func GracefulStop(handel func(http.ResponseWriter, *http.Request)) func(http.ResponseWriter, *http.Request) {
	return func(writer http.ResponseWriter, request *http.Request) {
		lock.Add(1)
		if isStop {
			//不再接收新的请求
			writer.WriteHeader(400)
			writer.Write([]byte("server is stop"))
		} else {
			handel(writer, request)
		}
		lock.Done()
	}
}

// 一个会耗时很长的方法
func LongTime(writer http.ResponseWriter, request *http.Request) {
	time.Sleep(time.Second * 5)
	writer.Write([]byte("Hello"))
}

func Normal(writer http.ResponseWriter, request *http.Request) {
	writer.Write([]byte("Hello"))
}

func main() {

	http.HandleFunc("/normal", Normal)
	http.HandleFunc("/long-time", GracefulStop(LongTime))

	//监听信号
	go func() {
		ch := make(chan os.Signal, 1)
		signal.Notify(ch, syscall.SIGINT, syscall.SIGTERM, syscall.SIGUSR1)
		for {
			sign := <-ch
			ctx, _ := context.WithTimeout(context.Background(), 20*time.Second)
			switch sign {
			case syscall.SIGINT, syscall.SIGTERM:
				{
					println("stop...")
					//停止
					isStop = true
					lock.Wait()
					//结束
					signal.Stop(ch)
					server.Shutdown(ctx)
					println("stop")
					return
				}
			case syscall.SIGUSR1:
				{
					println("reload...")
					//热重启
					tl, _ := listener.(*net.TCPListener)
					f, _ := tl.File()
					cmd := exec.Command(os.Args[0], "--reload")
					cmd.Stdout = os.Stdout
					cmd.Stdin = os.Stdin
					cmd.Stderr = os.Stderr
					cmd.ExtraFiles = []*os.File{f}
					cmd.Start()
					signal.Stop(ch)
					server.Shutdown(ctx)
					println("father stop")
					return
				}
			}
		}
	}()
	if isReload {
		f := os.NewFile(3, "")
		listener, _ = net.FileListener(f)
		println("child start")
	} else {
		listener, _ = net.Listen("tcp", ":8080")
	}
	server = &http.Server{
		Addr:    ":8080",
		Handler: nil,
	}

	server.Serve(listener)
}
