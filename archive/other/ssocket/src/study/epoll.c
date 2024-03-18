#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <malloc.h>
#include <mhash.h>
#include <fcntl.h>

#define MAX_EVENT 20
#define BUFF_LEN 1024

void log(const char *str) {
    printf("%s\n", str);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        log("socket create error");
        return 0;
    }
    //设置为非阻塞，因为ET模式的原因，一次accept可能会接收多个用户
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);

    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(1234);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (const struct sockaddr *) &server_sockaddr, sizeof(struct sockaddr_in)) == -1) {
        log("bind socket error");
        return 0;
    }
    listen(fd, SOMAXCONN);
    struct sockaddr_in client_sockaddr;
    int size = 0, client_fd = 0, fd_num = 0;
    int epfd = epoll_create(1);
    size_t ret_size;
    char *buf = malloc(BUFF_LEN);

    struct epoll_event ev, evs[MAX_EVENT];
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    int count = 0;
    while (1) {
        fd_num = epoll_wait(epfd, evs, MAX_EVENT, -1);
        printf("fd_num:%d\n", fd_num);
        for (int i = 0; i < fd_num; i++) {
            if (evs[i].data.fd == fd) {
                //server 收到链接,非阻塞循环accept,直到-1
                while ((client_fd = accept(fd,
                                           (struct sockaddr *) &client_sockaddr,
                                           (socklen_t *) &size)) != -1) {
                    ev.data.fd = client_fd;
                    ev.events = EPOLLIN | EPOLLET;
                    printf("connect:%d\n", client_fd);
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
                }
                if (errno != EAGAIN) {
                    printf("accept error\n");
                    return 0;
                }
            } else if (evs[i].events) {
                count++;
                //应该是客户端了
                client_fd = evs[i].data.fd;
                memset(buf, 0, BUFF_LEN);
                ret_size = (size_t) recv(client_fd, buf, BUFF_LEN, 0);
                printf("fd:%d,ret:%d,count:%d,recv\n", client_fd, ret_size, count);
                ret_size = send(client_fd, "hello", 5, 0);
                close(client_fd);
                ret_size = epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &ev);
            }
        }
    }
    return 0;
}
