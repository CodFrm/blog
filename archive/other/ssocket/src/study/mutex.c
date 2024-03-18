#include <stdio.h>
#include <sys/shm.h>
#include <mhash.h>
#include <pthread.h>

int main() {
    int shm = shmget(0, sizeof(int), 0666 | IPC_CREAT);
    int *a = shmat(shm, 0, 0);
    *a = 0;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
    //配置选项
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);//比较重要的一步,进程间共享
    pthread_mutex_init(&mutex, &mutexattr);
    for (int i = 0; i < 100; i++) {
        int pid = fork();
        if (pid == 0) {
            int *b = shmat(shm, 0, 0);
            while (true) {
                int ret = pthread_mutex_lock(&mutex);
                if (*b >= 10000) {
                    pthread_mutex_unlock(&mutex);
                    break;
                }
                *b = *b + 1;
                printf("pid:%d,%d,%d\n", getpid(), *b, ret);
                pthread_mutex_unlock(&mutex);
            }
            return 0;
        }
    }
    sleep(4);
    shmctl(shm, IPC_CREAT, 0);
    pthread_mutexattr_destroy(&mutexattr);
    pthread_mutex_destroy(&mutex);
    return 0;
}
