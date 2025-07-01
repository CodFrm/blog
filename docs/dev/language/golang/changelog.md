# Golang 版本变化

> [Release History](https://tip.golang.org/doc/devel/release)

持续更新golang版本更新变化内容，只记录了一些值得关注的内容，内容做了很大的精简，仅做知识索引。
(本文是从1.22才开始记录的，老的版本记录得比较随意）

如果你对某一点有兴趣，推荐去官网查看详细内容。

某些变更知道后还是挺有用的，例如我之前遇到过一个随机数的问题，本地没问题，线上总是生成一样的随机数，
排查了一段时间，才知道是因为没有设置随机数种子，我本地用的是go1.20，线上是go1.19，在1.20版本时，会自动生成一个随机数种子。

## 1.24

> [Go 1.24 Release Notes](https://tip.golang.org/doc/go1.24)

- 🔥 要求Linux内核3.2+ 不再支持Linux内核2.6.32
- 🔥 macOS支持变更 最后支持macOS 11 Big Sur的版本，1.25将要求macOS 12+
- 💥 完全支持泛型类型别名 类型别名现在可以像定义类型一样进行参数化
- 💥 移除SHA-1证书签名支持 `crypto/x509`不再支持SHA-1签名验证
- 💥 RSA密钥最小长度要求1024位 小于1024位的RSA密钥将返回错误
- ✨ 新增`os.Root`类型 提供目录限制的文件系统访问
- ✨ 新增`testing.B.Loop`方法 更快且不易出错的基准测试迭代方法
- ✨ 新增`runtime.AddCleanup`函数 比`SetFinalizer`更灵活高效的终结器机制
- ✨ 新增`weak`包 提供弱指针支持
- ✨ 新增`crypto/mlkem`包 实现ML-KEM-768和ML-KEM-1024后量子密钥交换
- ✨ 新增`crypto/hkdf`、`crypto/pbkdf2`、`crypto/sha3`包 从golang.org/x/crypto迁移
- ✨ 新增实验性`testing/synctest`包 支持并发代码测试
- ✨ JSON支持`omitzero`标签 比`omitempty`更清晰地省略零值
- ⚡️ 运行时性能提升2-3% 包括基于Swiss Tables的新map实现、更高效的小对象内存分配等

## 1.23

> [Go 1.23 Release Notes](https://tip.golang.org/doc/go1.23)

- 💥 `for range`支持迭代器函数 支持`func(func() bool)`、`func(func(K) bool)`、`func(func(K, V) bool)`类型的迭代器
- 💥 Timer和Ticker的重大变化
  - 未引用的Timer/Ticker可立即被垃圾回收，即使未调用Stop方法
  - timer channel变为无缓冲（容量0），保证Reset/Stop调用后不会收到过期值
- ✨ 新增`iter`包 提供用户定义迭代器的基本定义
- ✨ 新增`unique`包 提供值规范化功能（类似"interning"或"hash-consing"）
- ✨ 新增`structs`包 提供修改结构体属性的类型，如`HostLayout`标记符合主机平台期望的结构体布局
- ✨ `slices`包新增多个迭代器相关函数 如`All`、`Values`、`Backward`、`Collect`、`Sorted`、`Chunk`等
- ✨ `maps`包新增多个迭代器相关函数 如`All`、`Keys`、`Values`、`Insert`、`Collect`等

## 1.22

> [Go 1.22 Release Notes](https://tip.golang.org/doc/go1.22)

- 💥 `for range`不再是共享变量 这个变更可能会导致一些代码的行为发生变化
- 💥 支持`for i:=range 10`循环计数
- ✨ 新增`math/rand/v2`包 标准库的第一个v2版本包
- ✨ `http.ServeMux`路由增强

## 1.21

> [Go 1.21 Release Notes](https://tip.golang.org/doc/go1.21)

- 🔥 不再支持Windows 7
- 💥 `builtin`包新增`min`、`max`和`clear`函数
- 💥 支持WASI（WebAssembly System Interface）
- 💥 PGO默认开启
- 💥 GO团队对兼容性的承诺 [GODEBUG](https://go.dev/doc/godebug#history)
- ✨ 新增`log/slog`包、`slices`、`maps`和`cmp`包
- ✨ `context`包新增WithoutCancel、WithDeadlineCause、WithTimeoutCause函数

## 1.20

> [Go 1.20 Release Notes](https://tip.golang.org/doc/go1.20)

- 💥 引入PGO（Profile-guided optimization）
- ⚡️ slice可以直接转为数组 不再需要先转化为指针了
- ⚡️ 优化unsafe包的切片/字符串操作
- ✨ `context`包新增`WithCancelCause`函数 可以传递取消原因
- ✨ 新增`arena`包、`math/rand`自动全局随机数

## 1.19

> [Go 1.19 Release Notes](https://tip.golang.org/doc/go1.19)

- 💥 新的内存模型
- ✨ 软内存限制(soft memory limit)
- ✨ `atomic`添加多个新函数
- ✨ 新的编译约束`//go:build unix`

## 1.18

- 💥 泛型支持
- 💥 模糊测试(Fuzzing)
- 💥 工作区模式
- ⚡️ append切片扩容策略变更
- ✨ 新增`net/netip`包、新增`strings/bytes.Cut`函数、`sync`包新增`TryLock`方法

## 1.17

- ✨ 新增`//go:build`指令
- ✨ `go.mod`支持`Deprecated`指令
- ✨ 允许切片转化为数组
- ⚡️ Go Module优化

## 1.16

- 💥 原生支持Mac M1
- 💥 Go module转正
  - go build/run 不再自动更新go.mod和go.sum
  - 添加`retract`指示符 可以废弃模块
- 💥 新增`embed`包 支持嵌入二进制文件
- ✨ 新增`runtime/metrics`包、`io/fs`

## 1.15

- 🔥 不再支持`darwin`32位系统
- ✨ 加入新的`linker`工具
- ✨ 增加`time/tzdata`包，
- ⚡️ `panic`展现形式调整

## 1.14

- 💥 支持异步抢占式调度 `runtime.GOMAXPROCS(1)`也不会阻塞了
- ⚡️ 优化`defer`性能

## 1.13

- 💥 Go module成为默认的依赖管理工具
  - `GOPROXY`有默认初值并支持设置成多个代理的列表
  - `GOPRIVATE`私有化模块
- 💥 错误处理的改进，增加了`errors.Is`、`errors.As`、`errors.Unwarp`等函数和支持`fmt.Errorf`的`%w`
- 💥 支持数字字面量改进，引入二进制字面量，以及`_`作为数字分隔符
- ✨ `go.mod`文件中引入`go`指令，指定编译该模块所需的Go版本
