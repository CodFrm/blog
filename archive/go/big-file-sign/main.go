package main

import (
	"bytes"
	"crypto/md5"
	"fmt"
	"io"
	"net/http"
	"runtime"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	/**
	1G 文件测试结果
	{
	    "md5": "23023b24fc0ab2d03d5cd62a18bb4aa8",
	    "mem": "96M",
	    "startMem": "1M"
	}
	100M 文件测试结果
	{
	    "md5": "6ba2b5a1b62f356fc86efad690613916",
	    "mem": "96M",
	    "startMem": "0M"
	}
	*/
	r.GET("/hash1", func(c *gin.Context) {
		fmt.Printf("%v", c.ContentType())
		m := runtime.MemStats{}
		runtime.ReadMemStats(&m)
		startM := m.Alloc / 1024 / 1024
		oldBody := c.Request.Body
		defer oldBody.Close()
		pr, pw := io.Pipe()
		defer pw.Close()
		defer pr.Close()
		c.Request.Body = pr
		hash := md5.New()
		go func() {
			_, err := io.Copy(io.MultiWriter(hash, pw), oldBody)
			if err != nil {
				fmt.Printf("io copy: %v", err)
			}
		}()
		_, err := c.MultipartForm()
		if err != nil {
			c.AbortWithError(http.StatusInternalServerError, err)
			return
		}
		runtime.ReadMemStats(&m)
		c.JSON(http.StatusOK, gin.H{
			"md5":      fmt.Sprintf("%x", hash.Sum(nil)),
			"startMem": fmt.Sprintf("%dM", startM),
			"mem":      fmt.Sprintf("%dM", m.Alloc/1024/1024),
		})
	})

	/**
	1G 文件测试结果
	{
	    "md5": "62da2c499cdb3fad927f881c134684b0",
	    "mem": "2922M",
	    "startMem": "1M"
	}
	100M 文件测试结果
	{
	    "md5": "55a6849293d0847a48f856254aa910e2",
	    "mem": "341M",
	    "startMem": "1M"
	}
	*/
	r.GET("/hash2", func(c *gin.Context) {
		m := runtime.MemStats{}
		runtime.ReadMemStats(&m)
		startM := m.Alloc / 1024 / 1024
		oldBody := c.Request.Body
		defer oldBody.Close()
		buffer := bytes.NewBuffer(nil)
		hash := md5.New()
		_, err := io.Copy(io.MultiWriter(buffer, hash), oldBody)
		if err != nil {
			fmt.Printf("io copy2: %v", err)
		}
		c.Request.Body = io.NopCloser(buffer)
		_, err = c.MultipartForm()
		if err != nil {
			c.AbortWithError(http.StatusInternalServerError, err)
			return
		}
		runtime.ReadMemStats(&m)
		c.JSON(http.StatusOK, gin.H{
			"md5":      fmt.Sprintf("%x", hash.Sum(nil)),
			"startMem": fmt.Sprintf("%dM", startM),
			"mem":      fmt.Sprintf("%dM", m.Alloc/1024/1024),
		})
	})

	/**
	1G 文件测试结果
	{
	    "mem": "96M",
	    "startMem": "1M"
	}
	100M 文件测试结果
	{
	    "mem": "96M",
	    "startMem": "1M"
	}
	*/
	r.GET("/hash3", func(c *gin.Context) {
		m := runtime.MemStats{}
		runtime.ReadMemStats(&m)
		startM := m.Alloc / 1024 / 1024
		_, err := c.MultipartForm()
		if err != nil {
			c.AbortWithError(http.StatusInternalServerError, err)
			return
		}
		runtime.ReadMemStats(&m)
		c.JSON(http.StatusOK, gin.H{
			"startMem": fmt.Sprintf("%dM", startM),
			"mem":      fmt.Sprintf("%dM", m.Alloc/1024/1024),
		})
	})
	r.Run(":8088")
}
