---
title: c 协程
---

## 前言

协程可以说是用同步的代码写出异步的效果，前几天还看了异步，这些都算是在高性能系统中的一部分，都是压榨我们的 cpu，将 io 堵塞的时间去做其他事情。

异步的解决方式是执行立刻返回，我们的代码继续往下走，当完成之后通知我们。

协程的方式是代码执行立刻返回，之后我们将该条协程挂起，然后这段时间去执行其他的协程，等待 io 完成后恢复这条协程继续往下执行。相比于异步，至少在代码上就不会出现那种回调地狱了。

和线程相比，协程更加轻量，占用资源更少，通过协作的方式利用资源（因为是在单条线程内，不会同时执行）而不是抢占（多条线程可能同时执行读取某一数据），线程是由系统进行调度，协程是用户自己进行调度。

## 实现

在 c 中有好几种协程的实现方式，这里我使用 ucontext 实现

- 利用 switch-case 奇淫巧技实现
- asm 汇编实现
- 利用 c 的 setjmp 和 longjmp 函数实现
- ucontext 保存上下文实现
- boost.context

### ucontext 函数

先了解一下[ucontext](http://pubs.opengroup.org/onlinepubs/7908799/xsh/ucontext.h.html)的函数

```c
int  getcontext(ucontext_t *);
int  setcontext(const ucontext_t *);
void makecontext(ucontext_t *, (void *)(), int, ...);
int  swapcontext(ucontext_t *, const ucontext_t *);
```

### get/setcontext

[http://pubs.opengroup.org/onlinepubs/7908799/xsh/getcontext.html](http://pubs.opengroup.org/onlinepubs/7908799/xsh/getcontext.html)
用于获取当前和设置上下文

#### 例子

这个例子会一直输出 hello，和 goto 有点相似，不同的是，它可以在不同的函数之中进行跳转

getcontext 把当前的上下文保存到 ucp 中，后面使用 setcontext 恢复

```c
ucontext_t ucp;
void print() {
    printf("hello\n");
    setcontext(&ucp);
}
int main() {
    getcontext(&ucp);
    sleep(1);
    print();
    return 0;
}
```

### make/swapcontext

修改 getcontext 初始化的 ucp 上下文，调用 swapcontext 或 setcontext 恢复的时候程序将调用 func，可以自己分配堆栈，**uc_link**可以指定执行完后的上下文

swapcontext 将当前上下文保存在 ocup 上，并将上下文设置为 ucp

#### 例子

```c
void print() {
    printf("hello\n");
}

int main() {
    ucontext_t ucp, print_ucp;
    getcontext(&ucp);
    print_ucp = ucp;
    char stack[10 * 1204];
    print_ucp.uc_stack.ss_sp = stack;
    print_ucp.uc_stack.ss_size = sizeof(stack);
    print_ucp.uc_stack.ss_flags = 0;
    print_ucp.uc_link = &ucp;
    makecontext(&print_ucp, print, 0);

    swapcontext(&ucp, &print_ucp);
    printf("end\n");
    return 0;
}
```

### 协程

协程的一个关键就是上下文切换，在我们需要的时候切换至其他的协程

这里我只是写了一个非常简单的协程，只有创建，挂起，恢复功能

### 定义

定义了协程的状态，大小，协程函数指针，我的协程的结构体

**重点是我们的结构体里面，两个 ucontext_t，我们需要利用他们进行上下文的切换**

```c
#define STACK_SIZE 1024*128
#define CO_RUN 1
#define CO_HANG 2
#define CO_OVER 3

typedef void (*co_func)(struct coroutine *co);

struct coroutine {
    char stack[STACK_SIZE];//栈
    ucontext_t ctx;//协程上下文
    ucontext_t ucp;//主线程上下文
    char status;//协程状态
};
```

#### 创建协程

这里我没有直接的指向**func**因为还有一个完成状态要标记，所以我们定义了一个我们的协程主函数进行一些处理

我的创建协程主要是做的事情是，初始化上下文，调起函数

```c
void co_main(struct coroutine *co, co_func func) {
    func(co);
    co->status = CO_OVER;
}
void co_create(struct coroutine *co, co_func func) {
    getcontext(&co->ucp);
    co->ctx = co->ucp;
    co->ctx.uc_stack.ss_sp = co->stack;
    co->ctx.uc_stack.ss_size = STACK_SIZE;
    co->ctx.uc_stack.ss_flags = 0;
    co->ctx.uc_link = &co->ucp;
    co->status = CO_RUN;
    makecontext(&co->ctx, co_main, 2, co, func);//指向的co_main
    swapcontext(&co->ucp, &co->ctx);
}
```

### 挂起/恢复协程

挂起和恢复的时候我们都用 status 判断了一下是否执行结束

挂起和恢复就是上下文的交换，调用的**swapcontext**，挂起时将我们协程的上下文记录，切换到线程的上下文，交给线程去进行调度，恢复则相反

```c
void co_yield(struct coroutine *co) {
    if (co->status == CO_OVER) {
        return;
    }
    co->status = CO_HANG;
    swapcontext(&co->ctx, &co->ucp);
}

void co_resume(struct coroutine *co) {
    if (co->status == CO_OVER) {
        return;
    }
    co->status = CO_RUN;
    swapcontext(&co->ucp, &co->ctx);
}
```

### 完整源码

```c
#include <stdio.h>
#include <ucontext.h>
#include <mhash.h>

#define STACK_SIZE 1024*128
#define CO_RUN 1
#define CO_HANG 2
#define CO_OVER 3

typedef void (*co_func)(struct coroutine *co);

struct coroutine {
    char stack[STACK_SIZE];//栈
    ucontext_t ctx;//ucp
    ucontext_t ucp;
    char status;//协程状态
};

void co_main(struct coroutine *co, co_func func) {
    func(co);
    co->status = CO_OVER;
}

void co_create(struct coroutine *co, co_func func) {
    getcontext(&co->ucp);
    co->ctx = co->ucp;
    co->ctx.uc_stack.ss_sp = co->stack;
    co->ctx.uc_stack.ss_size = STACK_SIZE;
    co->ctx.uc_stack.ss_flags = 0;
    co->ctx.uc_link = &co->ucp;
    co->status = CO_RUN;
    makecontext(&co->ctx, co_main, 2, co, func);
    swapcontext(&co->ucp, &co->ctx);
}

void co_yield(struct coroutine *co) {
    if (co->status == CO_OVER) {
        return;
    }
    co->status = CO_HANG;
    swapcontext(&co->ctx, &co->ucp);
}

void co_resume(struct coroutine *co) {
    if (co->status == CO_OVER) {
        return;
    }
    co->status = CO_RUN;
    swapcontext(&co->ucp, &co->ctx);
}

int co_status(struct coroutine *co) {
    return co->status;
}

void print1(struct coroutine *co) {
    for (int i = 0; i < 50; i++) {
        printf("1号协程:%d\n", i);
        co_yield(co);
    }
}

void print2(struct coroutine *co) {
    for (int i = 100; i < 200; i++) {
        printf("2号协程:%d\n", i);
        co_yield(co);
    }
}

int main() {
    struct coroutine co1, co2;
    co_create(&co1, print1);
    co_create(&co2, print2);
    while (co_status(&co1) != CO_OVER || co_status(&co2) != CO_OVER) {
        co_resume(&co1);
        co_resume(&co2);
    }
    return 0;
}
```

### 栈

贴一个例子，将上面的第二个 swapcontext 那里的例子，print 换成下面这个，帮助大家理解一下栈，看输出，暂时就不写说明了

```c
void test() {
    int a = 1, len = ((int64_t) (&stack[10 * 1024 - 1]) - (int64_t) (&a)), b = 23;
    for (int i = 10 * 1024 - len - 8; i < 10 * 1024; i++) {
        if (i % 20 == 0) {
            printf("\n");
        }
        printf("%d\t", stack[i]);
    }
}

void print() {
    int a = 1, len = ((int64_t) (&stack[10 * 1024 - 1]) - (int64_t) (&a)), b = 23;
    printf("hello,%ld,%ld,%d\n", &a, &stack[10 * 1024 - 1], (int64_t) (&stack[10 * 1024 - 1]) - (int64_t) (&a));
    test();
//    for (int i = 10 * 1024 - len - 8; i < 10 * 1024; i++) {
//        if (i % 20 == 0) {
//            printf("\n");
//        }
//        printf("%d\t", stack[i]);
//    }
}
```
