# Golang IO相关操作

- io包提供了基础的io操作，几乎所有的io操作都是基于io.Reader和io.Writer接口的。
- ioutil 包提供了一些方便的io操作函数，但是在go 1.16中已经被废弃，放进了io包。
- bufio实现了缓冲io，可以提高io效率。
- bytes 包中提供了Buffer类型，可以用来做io操作。

## 核心接口

### Reader/Writer/Seeker

Reader和Writer是io包中最重要的接口，它们定义了io操作的基本行为。像一些文件操作，网络操作等等，都是基于这两个接口的。

Seeker接口定义了Seek方法，可以在Reader/Writer中定位到指定的位置，文件操作中常用。

## 常用函数

下面我们来看一些常用的io操作函数，更多的函数可以查看官方文档，我只是列出一些我常用到的。

- io.ReadeAll 读取所有数据，返回一个bytes
- io.MultiWriter/Reader 可以将多个Reader/Writer合并成一个
- io.LimitReader 限制Reader的读取长度，读取n个长度后返回io.EOF
- io.TeeReader 读取数据的同时写入到另一个Writer中
- io.Pipe 创建同步内存管道
- io.Copy/CopyN 复制Reader到Writer
- io.NopCloser 给一个io.Reader加上Close方法
- bytes.NewBuffer 创建一个Buffer
- bufio.NewReader/Writer 创建一个缓冲Reader/Writer

## 常见小坑

> 我暂时能想到的坑记录下来，欢迎补充

### io.EOF

io.EOF是io包中定义的一个错误，表示读取到文件末尾。在读取文件时，当读取到文件末尾时，会返回io.EOF错误。

请注意，当读取到文件末尾时，返回的数据可能不是nil，而是文件的最后一部分数据，需要注意处理。

### 大文件的操作

在处理大文件时，需要注意内存的使用，尽量使用io.Reader和io.Writer来处理文件，避免一次性读取整个文件到内存中。

### bufio.Writer.Flush

在使用bufio.Writer时，需要注意Flush方法，因为bufio.Writer是带缓冲的，数据并不是实时写入到Writer中的，需要调用Flush方法来刷新缓冲区。

### io.Pipe

io.Pipe是一个同步的内存管道，请注意在使用时，Reader与Writer必须在不同的goroutine中，且需要注意Reader与Writer的速度需要一致，否则会造成死锁或线程阻塞。

并且需要使用Close方法来关闭Reader或Writer，否则会造成内存泄漏。

## 高级用例

下面介绍一些高级用例，你也可以从中学习到一些高级的io操作技巧。

### 计算上传文件的hash

下面的例子是计算上传文件的md5和sha1的hash值，我们使用io.TeeReader将文件内容同时写入到md5和sha1的hash计算器中。
像我们在http上传文件时，可以在上传的同时计算文件的hash值，这样可以保证文件的完整性。同时也可以打开文件，再将文件的Writer放到io.Copy中，这样可以同时写入文件和计算hash值。

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

下面的例子是读取文件的部分内容，我们使用io.LimitReader限制读取的长度，读取n个长度后返回io.EOF，这在解析文件头部信息时非常有用。

```go
func TestLimitReader(t *testing.T) {
 file := bytes.NewBuffer([]byte("hello world"))
 lr := io.LimitReader(file, 5)
 data, _ := io.ReadAll(lr)
 t.Logf("%s\n", data)
}
```

### 大文件按行读取

下面的例子是读取大文件的每一行，我们使用bufio.Reader来读取文件的每一行，这样可以避免一次性读取整个文件到内存中。

在读一些io比较慢的文件时（网络文件），也可以使用这种方式来读取文件。写文件也是类似的，可以使用bufio.Writer来写文件，提高磁盘io效率，避免频繁的io操作。

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

### 使用Pipe将上传文件解密写入文件

有时候上传的文件是一个加密的，我们需要在上传的同时解密文件，然后同时写入到磁盘中，这时候可以使用io.Pipe来实现。

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
