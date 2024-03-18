#include <stdio.h>
#include <sys/shm.h>
#include <mhash.h>

int main() {
    int shm = shmget(0, sizeof(int), 0666 | IPC_CREAT);
    int *a = shmat(shm, 0, 0);
    *a = 6;
    for (int i = 0; i < 2; i++) {
        int pid = fork();
        if (pid == 0) {
            int *b = shmat(shm, 0, 0);
            printf("sub:%d,%d\n", *b, b);
            if (i == 1) {
                *b = 109;
                printf("sub1\n");
            }
            sleep(2);
            printf("sub:%d,%d\n", *b, b);
            return 0;
        }
    }
    sleep(1);
    *a = 10987;
    printf("father:%d,%d", *a, a);
    sleep(2);
    shmctl(shm, IPC_CREAT, 0);
    return 0;
}
