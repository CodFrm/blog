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
