package code

import (
	"bufio"
	"bytes"
	"compress/gzip"
	"crypto/md5"
	"crypto/sha1"
	"io"
	"testing"
)

type WrapReader struct {
	len int
	r   io.Reader
}

func (w *WrapReader) Read(p []byte) (int, error) {
	n, err := w.r.Read(p)
	w.len += n
	return n, err
}

func TestReader(t *testing.T) {
	file := bytes.NewBuffer([]byte("hello world"))
	wr := WrapReader{len: 0, r: file}
	_, _ = io.Copy(io.Discard, &wr)
	t.Logf("文件长度: %d\n", wr.len)
}

func TestGzip(t *testing.T) {
	file := bytes.NewBuffer([]byte("hello world"))
	buf := bytes.NewBuffer(nil)
	w := gzip.NewWriter(buf)
	_, _ = io.Copy(w, file)
	t.Logf("不Close文件长度: %d\n", buf.Len())
	w.Close()
	t.Logf("压缩后文件长度: %d\n", buf.Len())
}

func TestIOHash(t *testing.T) {
	file := bytes.NewBuffer([]byte("hello world"))
	md5 := md5.New()
	sha1 := sha1.New()
	r := io.TeeReader(file, io.MultiWriter(md5, sha1))
	_, _ = io.Copy(io.Discard, r)
	t.Logf("md5: %x\n", md5.Sum(nil))
	t.Logf("sha1: %x\n", sha1.Sum(nil))
}

func TestLimitReader(t *testing.T) {
	file := bytes.NewBuffer([]byte("hello world"))
	lr := io.LimitReader(file, 5)
	data, _ := io.ReadAll(lr)
	t.Logf("%s\n", data)
}

func TestBufIo(t *testing.T) {
	// 写入100M文件
	file := bytes.NewBufferString("hello world\ngolang")
	buf := bufio.NewReader(file)
	for {
		line, err := buf.ReadString('\n')
		if err == io.EOF {
			t.Logf("%s", line)
			break
		}
		t.Logf("%s", line)
	}
}

// 凯撒加密
type decode struct {
	r io.Reader
}

func (d *decode) Read(p []byte) (int, error) {
	// 读取数据
	n, err := d.r.Read(p)
	if err != nil {
		if err != io.EOF {
			return n, err
		}
	}
	// 解密
	for i := 0; i < n; i++ {
		p[i]++
	}
	return n, err
}

func TestDecodePipe(t *testing.T) {
	body := bytes.NewBuffer([]byte("gdkkn\u001Fvnqkc"))
	r, w := io.Pipe()
	defer r.Close()
	go func() {
		defer w.Close()
		// 读取加密数据解密
		_, _ = io.Copy(w, &decode{r: body})
	}()
	// 读取解密数据，保存文件
	data, _ := io.ReadAll(r)
	t.Logf("%s\n", data)
}
