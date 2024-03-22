---
title: SMTP+SSL协议研究-PHP实现
---

> 突然的就想尝试一下实现邮件发送协议,尤其是 SMTP+SSL 之类的方式,SMTP 协议全是明文的,写起来倒是不困难,但是到现在还完全不了解+SSL 的工作方式

## 开头

github:https://github.com/huanl-php/protocol

打算以后将实现的协议都放在这里,所以要做好规划

### Socket

[php socket](http://www.php.net/sockets "php socket")

先了解好 php 的 socket 函数,和 c 的 socket 非常像.为什么这里我们使用 socket 来实现,而不是用 swoole,因为在大多数的情况下,swoole 扩展并不一定安装了,这是非常不方便的

### client 类

> 这个是用来连接服务器的,由这一个类扩展出其他的协议,需要在这个里面写好连接和发送,接收的一些功能

这里不贴代码了,可以去 github 看:https://github.com/huanl-php/protocol/blob/master/src/Client.php

### smtp

> 这里我拿我的阿里云的邮箱测试,参考这篇文章 [邮件实现详解（二）------手工体验 smtp 和 pop3 协议](https://www.cnblogs.com/ysocean/p/7653252.html#_label0),顺便学习了一波 telnet,这篇文章是真的详细,我就不写过程了



![](img/SMTP+SSL%E5%8D%8F%E8%AE%AE%E7%A0%94%E7%A9%B6-PHP%E5%AE%9E%E7%8E%B0.assets/TIM%E6%88%AA%E5%9B%BE20180822150356-300x201.png)



### 实现

[SMTP](https://github.com/huanl-php/protocol/blob/master/src/SMTP.php)代码在这里了- -...然后看看 SSL...的工作方式

## SSL

> SSL 相当于是中间的一套层,客户端发送消息经过 SSL 层加密发送给服务器,然后经过服务器的 SSL 层又解密给服务器

在 php 中,ssl 套层实现非常的简单...用`stream_socket_client`连接 server,然后使用`stream_socket_enable_crypto`设置 ssl 链接,之后`fwrite`发送数据(大概这就是 linux 的哲学**万物皆文件**的体现吧),如果是这样,那么只需要在 Client 类中,重写一次 connect 和 send 那些方法就够了

### 源码

[SSLClient](https://github.com/huanl-php/protocol/blob/master/src/SSLClient.php)
