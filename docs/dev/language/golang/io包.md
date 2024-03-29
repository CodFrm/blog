# Golang IO 相关操作

- io 包提供了基础的 io 操作，几乎所有的 io 操作都是基于 io.Reader 和 io.Writer 接口的。
- ~~ioutil 包提供了一些方便的 io 操作函数，但是在 go 1.16 中已经被废弃，放进了 io 包。~~
- bufio 实现了缓冲 io，可以提高 io 效率。
- bytes 包中提供了 Buffer 类型，可以用来做 io 操作。

## 核心接口

Go 中几乎所有的 io 操作都是围绕着 Reader/Writer/Seeker 这三个接口进行，这三个接口都是独立开来的，可以随机组合。

Reader 和 Writer 是 io 包中最重要的接口，它们定义了 io 操作的基本行为。像一些文件操作，网络操作等等，都是基于这两个接口的。

Seeker 接口定义了 Seek 方法，可以在 Reader/Writer 中定位到指定的位置，文件操作中常用。

还有一个 Close 接口，用来关闭 io，也是经常使用，有这个接口的时候，我们可以使用 defer 来关闭，避免忘记，而且一般情况下都需要注意关闭，否则会产生内存泄漏之类的问题。

有很多操作也是需要执行完 Close 之后才能生效，例如 gzip 压缩，如果不 Close，可能会导致压缩文件不完整。

```go
func TestGzip(t *testing.T) {
 file := bytes.NewBuffer([]byte("hello world"))
 buf := bytes.NewBuffer(nil)
 w := gzip.NewWriter(buf)
 _, _ = io.Copy(w, file)
 t.Logf("不Close文件长度: %d\n", buf.Len())
 w.Close()
 t.Logf("压缩后文件长度: %d\n", buf.Len())
}
```

### 基础使用

别看这三个接口简单，他们可以玩出很多花出来。例如对 Reader/Writer 进行包装，实现读取写入大小统计，来做流量统计时这很有用；标准库中也有很多方法，值得可以学习。

```go
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
```

## 常用函数

下面我们来看一些常用的 io 操作函数，更多的函数可以查看官方文档，我只是列出一些我常用到的。

- io.ReadAll 读取所有数据，返回一个 bytes
- io.MultiWriter/Reader 可以将多个 Reader/Writer 合并成一个
- io.LimitReader 限制 Reader 的读取长度，读取 n 个长度后返回 io.EOF
- io.TeeReader 读取数据的同时写入到另一个 Writer 中
- io.Pipe 创建同步内存管道
- io.Copy/CopyN 复制 Reader 到 Writer
- io.NopCloser 给一个 io.Reader 加上 Close 方法
- bytes.NewBuffer 创建一个 Buffer
- bufio.NewReader/Writer 创建一个缓冲 Reader/Writer

## 常见小坑

> 我暂时能想到的坑记录下来，欢迎补充

### io.EOF

io.EOF 是 io 包中定义的一个错误，表示读取到文件末尾。在读取文件时，当读取到文件末尾时，会返回 io.EOF 错误。

请注意，当读取到文件末尾时，返回的数据可能不是 nil，而是文件的最后一部分数据，需要注意处理。

### 大文件的操作

在处理大文件时，需要注意内存的使用，尽量使用 io.Reader 和 io.Writer 来处理文件，避免一次性读取整个文件到内存中。

### bufio.Writer.Flush

在使用 bufio.Writer 时，需要注意 Flush 方法，因为 bufio.Writer 是带缓冲的，数据并不是实时写入到 Writer 中的，需要调用 Flush 方法来刷新缓冲区。

### io.Pipe

io.Pipe 是一个同步的内存管道，请注意在使用时，Reader 与 Writer 必须在不同的 goroutine 中，且需要注意 Reader 与 Writer 的速度需要一致，否则会造成死锁或线程阻塞。

并且需要使用 Close 方法来关闭 Reader 或 Writer，否则会造成内存泄漏。

## 高级用例

下面介绍一些高级用例，你也可以从中学习到一些高级的 io 操作技巧。

### 计算上传文件的 hash

下面的例子是计算上传文件的 md5 和 sha1 的 hash 值，我们使用 io.TeeReader 将文件内容同时写入到 md5 和 sha1 的 hash 计算器中。
像我们在 http 上传文件时，可以在上传的同时计算文件的 hash 值，这样可以保证文件的完整性。同时也可以打开文件，再将文件的 Writer 放到 io.Copy 中，这样可以同时写入文件和计算 hash 值。

```go
func TestIOHash(t *testing.T) {
 file := bytes.NewBuffer([]byte("hello world"))
 md5 := md5.New()
 sha1 := sha1.New()
 r := io.TeeReader(file, io.MultiWriter(md5, sha1))
 _, _ = io.Copy(io.Discard, r)
 t.Logf("md5: %x\n", md5.Sum(nil))
 t.Logf("sha1: %x\n", sha1.Sum(nil))
}
```

### 读取文件的部分内容

下面的例子是读取文件的部分内容，我们使用 io.LimitReader 限制读取的长度，读取 n 个长度后返回 io.EOF，这在解析文件头部信息时非常有用。

```go
func TestLimitReader(t *testing.T) {
 file := bytes.NewBuffer([]byte("hello world"))
 lr := io.LimitReader(file, 5)
 data, _ := io.ReadAll(lr)
 t.Logf("%s\n", data)
}
```

### 大文件按行读取

下面的例子是读取大文件的每一行，我们使用 bufio.Reader 来读取文件的每一行，这样可以避免一次性读取整个文件到内存中。

在读一些 io 比较慢的文件时（网络文件），也可以使用这种方式来读取文件。写文件也是类似的，可以使用 bufio.Writer 来写文件，提高磁盘 io 效率，避免频繁的 io 操作。

```go
func TestBufIo(t *testing.T) {
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
```

### 使用 Pipe 将上传文件解密写入文件

有时候上传的文件是一个加密的，我们需要在上传的同时解密文件，然后同时写入到磁盘中，这时候可以使用 io.Pipe 来实现。

```go

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
```
