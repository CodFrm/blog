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