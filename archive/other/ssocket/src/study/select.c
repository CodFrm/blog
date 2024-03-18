#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <mhash.h>

#define MAX_CLIENT 1023
#define BUFF_LEN 1024

void log(const char *str) {
    printf("%s\n", str);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (fd == -1) {
        log("socket create error");
        return 0;
    }
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(1234);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (const struct sockaddr *) &server_sockaddr, sizeof(struct sockaddr_in)) == -1) {
        log("bind socket error");
        return 0;
    }
    listen(fd, 20);
    struct sockaddr_in client_sockaddr;
    int client_fd[MAX_CLIENT] = {-1}, size;
    memset(client_fd, -1, MAX_CLIENT * sizeof(int));
    char *buf = malloc(BUFF_LEN);

    //select
    fd_set allset;
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    FD_ZERO(&allset);
    FD_SET(fd, &allset);
    fd_set tmp_set;
    int maxfd = fd;
    int count = 0;
    while (1) {
        tmp_set = allset;
        int ret = select(maxfd + 1, &tmp_set, NULL, NULL, NULL);
        if (ret > 0) {
            //大于0判断是否有数据进入了
            if (FD_ISSET(fd, &tmp_set)) {
                //如果是fd这个，那么代表有新链接进入
                int tmp_client_fd = accept(fd, (struct sockaddr *) &client_sockaddr, &size);
                //将客户fd存入数组
                int i = 0;
                for (i = 0; i < MAX_CLIENT; i++) {
                    if (client_fd[i] == -1) {//就是你了
                        client_fd[i] = tmp_client_fd;
                        break;
                    }
                }
                if (client_fd[i] != tmp_client_fd) {
                    //超出限制了,直接丢弃，因为select一般最多只能操作1024个文件
                    //__FD_SETSIZE决定的，更多需要修改内核
                    close(tmp_client_fd);
                } else {
                    if (client_fd[i] > maxfd) {
                        maxfd = client_fd[i];
                    }
                    FD_SET(client_fd[i], &allset);
                }
            } else {
                //不是服务器的句柄，那么就是客户端的句柄咯，要遍历客户端句柄
                maxfd = fd;
                for (int i = 0; i < MAX_CLIENT; i++) {
                    if (client_fd[i] != -1) {
                        if (FD_ISSET(client_fd[i], &tmp_set)) {
                            //是这个客户端，接受消息
                            memset(buf, 0, BUFF_LEN);
                            count++;
                            ret = recv(client_fd[i], buf, BUFF_LEN, 0);
                            printf("fd:%d,ret:%d,count:%d,recv\n", client_fd[i], ret, count);
                            if (ret == 0) {
                                //从集合中删除
                                printf("error,fd:%d,ret:%d,count:%d,recv\n", client_fd[i], ret, count);
                                close(client_fd[i]);
                                FD_CLR(client_fd[i], &allset);
                                client_fd[i] = -1;
                                continue;
                            }
                            ret = (int) send(client_fd[i], "hello", 5, 0);
                            //从集合中删除
                            close(client_fd[i]);
                            FD_CLR(client_fd[i], &allset);
                            client_fd[i] = -1;
                        }
                        if (client_fd[i] > maxfd) {
                            maxfd = client_fd[i];
                        }
                    }
                }
            }
        } else {
            break;
        }
    }
    free(buf);
    return 0;
}
