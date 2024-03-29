---
title: linux aio 异步io
---

> linux 下的 aio 有 glibc 的和内核所提供的，glibc 是使用的多线程的模式模拟的，另外一种是真正的内核异步通知了，
> 已经使用在了 nginx 上，前面看了一下 swoole 的实现，是类似与 glibc 那种多线程的模式。
> 不过两种方法都有一定的毛病，多线程模拟自然是有所效率损失，然而内核不能利用系统的缓存，只能以 O_DIRECT 方式做直接 IO，所以看知乎上有一个
> **linux 下的异步 IO（AIO）是否已成熟？** 的问题，不过那是 2014 年的事情了，不知道现在怎么样。

在此之前需要安装好 **libaio**

```bash
sudo apt install libaio-dev
```

## 函数

头文件:`#include <libaio.h>`

## 例子

> 一个一步读取文件内容的例子

```c
#include <stdio.h>
#include <fcntl.h>
#include <libaio.h>
#include <malloc.h>
#include <mhash.h>

#define MAX_EVENT 10
#define BUF_LEN 1024

void callback(io_context_t ctx, struct iocb *iocb, long res, long res2) {
    printf("test call\n");
    printf("%s\n", iocb->u.c.buf);
}

int main() {
    int fd = open("/home/huanl/client.ovpn", O_RDONLY, 0);
    io_context_t io_context;
    struct iocb io, *p = &io;
    struct io_event event[MAX_EVENT];
    char *buf = malloc(BUF_LEN);
    memset(buf, 0, BUF_LEN);
    memset(&io_context, 0, sizeof(io_context));

    if (io_setup(10, &io_context)) {
        printf("io_setup error");
        return 0;
    }
    if (fd < 0) {
        printf("open file error");
        return 0;
    }
    io_prep_pread(&io, fd, buf, BUF_LEN, 0);
    io_set_callback(&io, callback);
    if (io_submit(io_context, 1, &p) < 0) {
        printf("io_submit error");
        return 0;
    }

    int num = io_getevents(io_context, 1, MAX_EVENT, event, NULL);
    for (int i = 0; i < num; i++) {
        io_callback_t io_callback = event[i].data;
        io_callback(io_context, event[i].obj, event[i].res, event[i].res2);
    }

    return 0;
}
```

## 参考

[https://jin-yang.github.io/post/linux-program-aio.html](https://jin-yang.github.io/post/linux-program-aio.html)
