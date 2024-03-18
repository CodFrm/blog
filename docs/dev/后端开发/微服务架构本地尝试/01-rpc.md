---
title: 微服务架构本地尝试(一)-RPC
---

**本系列在 github 中更新,源码和 docker 都可在 github 项目中找到:[https://github.com/CodFrm/learnMicroService](https://github.com/CodFrm/learnMicroService)**
**GitHub 文章:[https://github.com/CodFrm/learnMicroService/blob/master/doc/%E5%BE%AE%E6%9C%8D%E5%8A%A1%E6%9E%B6%E6%9E%84%E6%9C%AC%E5%9C%B0%E5%B0%9D%E8%AF%95(%E4%B8%80)-rpc.md](<https://github.com/CodFrm/learnMicroService/blob/master/doc/%E5%BE%AE%E6%9C%8D%E5%8A%A1%E6%9E%B6%E6%9E%84%E6%9C%AC%E5%9C%B0%E5%B0%9D%E8%AF%95(%E4%B8%80)-rpc.md>)**



> 微服务架构最近非常的流行,我的公司年后也准备使用微服务架构.而且之前我也很想学习接触架构层面的知识,了解一些比较前沿的东西,所以利用这个假期来尝试学习一下微服务架构.
>
> 之前我已经了解了微服务大概是一个怎么样的东西,对于一些理论的东西并不打算写太多.个人觉得微服务的技术核心在于 rpc 和服务发现/注册;思想主要是不同业务之间的拆分
>
> 我打算尝试使用 golang 和 gRPC 框架一步一步的去摸索,带着一些问题去实践



## RPC

RPC（Remote Procedure Call）—远程过程调用,简单来说就是,在 A 服务器上调用 B 服务器上的方法.rpc 可以通过 http 协议但不仅限于来实现.大多 rpc 框架也会支持多种协议.利用 rpc 框架可以不让我们关注 rpc 实现层,让我们调用一个远程的方法就像在本地调用一样.

通常会将登录/注册(权限服务)拆分为一个微服务,当有一些操作(另外的微服务)需要验证某项权限时,可以通过 rpc 调用权限微服务上的方法来验证当前用户是否拥有权限.感觉像是通过 rpc 来将各个独立的微服务关联起来.

### [gRPC](https://grpc.io/about/)

[gRPC](https://grpc.io/about/)是 google 的一个跨语言的 rpc 框架.其实一开始是想用[rpcx](http://rpcx.site/)的,虽然中文文档挺齐全的,而且也是国人开发的,但是一搜索网上都没有什么资料,就暂时不了解了.

### protocol buffers

[protocol buffers](https://developers.google.com/protocol-buffers/docs/proto3)是一种轻便高效的结构化数据储存结构,这是 gRPC 默认的数据协议.我们也需要去了解一下它.

### 开始

先装好 golang 环境,获取包,先用官方提供给我们的例子试一试

```sh
# 安装grpc
go get -u google.golang.org/grpc
# 安装Protocol Buffers v3
go get -u github.com/golang/protobuf/protoc-gen-go
```

### 例子分析

我在 [client](https://github.com/CodFrm/learnMicroService/tree/master/examples/rpc/helloworld/greeter_client/main.go),[server](https://github.com/CodFrm/learnMicroService/tree/master/examples/rpc/helloworld/greeter_server/main.go)和[helloworld.pb.go](https://github.com/CodFrm/learnMicroService/tree/master/examples/rpc/helloworld/helloworld/helloworld.pb.go)的源码中做了一些注释,方法都可以在官方文档中看到[grpc](https://godoc.org/google.golang.org/grpc)

```sh
# 将例子复制过来
cp -r $GOPATH/src/google.golang.org/grpc/examples examples/rpc
# 编译例子 server和client 然后运行
cd examples/rpc/helloworld
```

默认端口是 50051,我竟然不能打开,好像是被占用了,我直接就换了一个,完成.

先运行服务端,然后运行客户端可以看到效果

![](img/01-rpc.assets/rpc_c_s-300x144.png)

helloworld.pb.go 是通过的插件`protoc-gen-go`编译`helloworld.proto`生成的.虽然可以自动生成,但我还想了解一下实现的方法.然后还需要了解 proto 的语法,我们才能制作属于我们的 rpc 调用接口

### 定义接口

#### protoc

这里就不弄太复杂了,我们可以参照例子给我们的来写,来写一个简单的,就比如权限验证,通过 token 和接口名字获取该用户信息和是否有权限使用接口.

```sh
syntax = "proto3"; # 定义版本
package = lms; # 包名

service UserAuth { # 定义服务
  # 验证token(TokenMsg),然后返回用户信息(UserMsg)
  rpc isvalid(TokenMsg) returns (UserMsg) {}
}

message TokenMsg { # 消息模型
    string token = 1; # string 字符串类型 token 名字
    string api = 2;
}

message UserMsg {
  int32 uid = 1;
  bool access =2;
  string name = 3;
  string group = 4;
}
```

更复杂的消息结构还能嵌套,枚举,这里就先只用这一些

上面的服务定义是 单项 RPC 的形式,调用完就没了,还有 服务端流式 RPC,客户端流式 RPC 和客户端流式 RPC,具体的可以去看一下其他的文章[https://colobu.com/2017/04/06/dive-into-gRPC-streaming/](https://colobu.com/2017/04/06/dive-into-gRPC-streaming/),感觉通常单项 RPC 就可以了,其他的应该是在一些数据传输的场景使用.

#### 生成 go 文件

写好 proto 文件后,我们需要用工具编译成 golang 代码.windows 需要去下载工具[https://github.com/protocolbuffers/protobuf/releases](https://github.com/protocolbuffers/protobuf/releases)

```sh
protoc -I ./proto --go_out=plugins=grpc:./proto ./proto/learnMicroService.proto
```

### 编写服务

继续参照给我们的例子写,具体代码看我源码吧 [auth 权限验证微服务](https://github.com/CodFrm/learnMicroService/tree/master/auth/main.go)

这里我构建了一个权限验证的微服务,然后另外构建一个发帖的,可以去看我的源码 [post 帖子微服务](https://github.com/CodFrm/learnMicroService/tree/master/post/main.go)

主要在发帖的时候判断是否拥有权限

```go
ctx, cancel := context.WithTimeout(context.Background(), time.Second)
defer cancel()
userMsg, err := authService.Isvalid(ctx, &micro.TokenMsg{
	Token: req.PostFormValue("token"),
	Api:   "post",
})
if err != nil {
	ret = "rpc调用错误"
} else if !userMsg.Access {
	ret = "没有权限"
} else {
	ret = userMsg.Name + "post 请求成功"
	posts = append(posts, req.PostFormValue("title"))
}
```

我用 postman 测试:

![](img/rpc.assets/rpc_debug_1-300x120.png)





![](img/rpc.assets/rpc_debug_2-300x197.png)



到这里,两个非常非常简陋的微服务算是完成了,一个权限验证,一个发帖 hhhh

