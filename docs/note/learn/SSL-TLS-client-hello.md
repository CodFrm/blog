---
title: SSL/TLS client hello 解析
---

> 摘抄:SSL 因为应用广泛，已经成为互联网上的事实标准。IETF 就在那年把 SSL 标准化。标准化之后的名称改为 TLS（是“Transport Layer Security”的缩写），中文叫做“传输层安全协议

> 上次写了个 ssl 的 smtp 协议,但是 ssl 实现那里,php 只需要随便调用几个函数就好了,觉得不过瘾,所以这次来看一下 ssl 的实现

## 准备

我们需要一个抓包工具**Wireshark**

![](img/SSL-TLS-client-hello.assets/TIM%E6%88%AA%E5%9B%BE20180828092927.png)

这是我捕获到的,为了方便,我是抓的我的博客,右键刷新源码,然后就停止抓包

上面是过滤内容,ip 地址等于我的博客的地址,并且是 ssl 协议(tls)

在此之前我看了不少的相关知识,但是都只是说应用和流程,好处什么的,还有说一堆算法的,老夫看不懂,老夫才不管这些什么,老夫写代码就是一把梭

阮老师这篇文章说得挺容易理解[图解 SSL/TLS 协议](http://www.ruanyifeng.com/blog/2014/09/illustration-ssl.html)

但是没有找到实现,后来谷歌搜了一下,才找到了一篇[client hello](https://blog.csdn.net/leinchu/article/details/80196025)解析的(再一次吐槽百度),通过这一篇文章我才有一点头绪

## SSL 握手

> 经过了 TCP 三次握手之后,就开始 SSL 的握手

### 结构

> 先了解一下大概结构

我们用 Wireshark 抓到了数据,写得非常的详细,看第一个包就是**client hello**,点它,然后点**secure sockets layer**这就是我们 ssl 的数据了,然后会帮我们选中我们的数据,非常的直接,前面那些应该是 tcp 协议相关的数据

![](img/SSL-TLS-client-hello.assets/TIM%E6%88%AA%E5%9B%BE20180828142314.png)

看这个,感觉大概结构应该是这样,首先是包类型版本号和长度,然后是内容

```cpp
struct ssl_handshake{
    char type;
    short version;
    short length;
    char *content;
}
```

握手包,点开 handshake Protocol 可以看到,client hello 和 server hello,前一部分都是这样

```cpp
//随机数
struct ssl_random{
    int timestamp;
    char random[28];
}
struct ssl_hello{
    char type;
    char length[3];
    short version;
    struct ssl_random random;
}
```

这个结构我写到了随机数截止,因为后面的大多是不同的且变化

#### session

**session**,这个是用来复用的,我记得这个只是一个记录,并不一定需要,于是我通过其他软件访问网页,抓到了一个没有 session 的包来比对,所以我们就可以直接在后面填充一个 0,然后继续后面的

![](img/SSL-TLS-client-hello.assets/TIM%E6%88%AA%E5%9B%BE20180829110033.png)



#### cipher suites

**cipher suites**应该是客户端支持的秘钥类型,里面应该都是一些常量值

意义,例如:TLS***ECDHE*****RSA**_WITH_**AES_128_GCM**\_ **SHA256** (0xc02f)
**ECDHE**秘钥交换算法
**RSA**身份验证算法
**AES_128_GCM**对称加密算法
**SHA256**摘要算法
这样按照顺序拆分开来看....

值的话,我只从 RFC 里面找到了零零散散的,不过我想干脆就从我们抓到的包里面提取几个吧

```
Cipher Suite: TLS_RSA_WITH_AES_128_GCM_SHA256 (0x009c)
Cipher Suite: TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 (0xc02f)
Cipher Suite: TLS_RSA_WITH_AES_128_CBC_SHA256 (0x003c)
```

#### Extension

> 跳过了 Compression Methods,等下按照我们抓到的值填就好了

Extension 听名字就知道是扩展项,感觉和原来的 radius 的一样

```cpp
struct ssl_extension{
    short type;
    short length;
    char *content;
}
```

我想先吧他们复制一下试试能不能行(偷懒,滑稽)

### client hello

> 客户端先给服务端打声招呼,告诉客户端支持的加密算法 balabala,先尝试写代码,发送一个 hello 的包给服务器看看回答

代码如下:

```php
    public function testSSLHello() {
        $str = SSL::pac_ssl_handshake(22, SSL::TLSv3,
            SSL::pack_ssl_hello(1, SSL::TLSv3, SSL::pack_ssl_random(),
                hex2bin('00') . SSL::pack_ciphersuites(['009c', 'c02f', '003c']) .
                hex2bin('0100005800000014001200000f626c6f672e69636f6465662e636f6d000500050100000000000a00080006001d00170018000b00020100000d001400120401050102010403050302030202060106030023000000170000ff01000100')
            )
        );//那一大串十六进制是我复制的扩展区
        static::$client->send($str);
        static::$client->recv($buf, 2048);
    }
```

发送之后成功接收到了服务器的应答,扩展区里面还包含了服务器的一些信息,如果换服务器可能还要修改一下`Extension: server_name`

~~今天坑先挖到这里,明天看 server hello~~

## 参考

[https://blog.csdn.net/mrpre/article/details/77867439](https://blog.csdn.net/mrpre/article/details/77867439)
[图解 SSL/TLS 协议](http://www.ruanyifeng.com/blog/2014/09/illustration-ssl.html)
[client hello](https://blog.csdn.net/leinchu/article/details/80196025)
