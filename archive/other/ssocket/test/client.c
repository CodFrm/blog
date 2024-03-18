#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <malloc.h>
#include <mhash.h>
#include <sys/wait.h>

#define FORK_NUM 10

int sub_pid[FORK_NUM] = {0};

void sigint_deal(int sig) {
    for (int i = 0; i < FORK_NUM; i++) {
        kill(sub_pid[i], SIGINT);
    }
    exit(0);
}

int main() {
    struct hostent *host = gethostbyname("127.0.0.1");
    void *buff = malloc(1024);

    signal(SIGINT, sigint_deal);
    for (int i = 0; i < FORK_NUM; i++) {
        int pid = fork();
        if (pid == 0) {
            //子进程
            printf("in sub process\n");
            for (int j = 0; j < 100; j++) {
                int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                struct sockaddr_in server_sock;
                server_sock.sin_family = AF_INET;
                server_sock.sin_port = htons(1234);
                server_sock.sin_addr = *((struct in_addr *) host->h_addr);
                int ret = connect(fd, (const struct sockaddr *) &server_sock, sizeof(struct sockaddr_in));
                if (ret == -1) {
                    printf("connect error\n");
                    continue;
                }
                ret = (int) send(fd, "test", 4, 0);
                printf("pid:%d,send test:%d\n", getpid(), ret);
                memset(buff, 0, 1024);
                ret = (int) recv(fd, buff, 1024, 0);
                printf("pid:%d,recv:%s\n", getpid(), (char *) buff);
                close(fd);
            }
            printf("sub process over\n");
            return 0;
        } else if (pid > 0) {
            sub_pid[i] = pid;
            printf("my father\n");
        } else {
            printf("create process error\n");
        }
    }
    for (int i = 0; i < FORK_NUM; i++) {
        wait((int *) sub_pid[i]);
    }
    printf("over\n");
    free(buff);
    return 0;
}