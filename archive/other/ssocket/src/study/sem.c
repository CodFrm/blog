#include <stdio.h>
#include <sys/shm.h>
#include <mhash.h>
#include <semaphore.h>

int main() {
    sem_t sem_id;
    sem_init(&sem_id, 1654, 1);
    int shm = shmget(0, sizeof(int), 0666 | IPC_CREAT);
    int *a = shmat(shm, 0, 0);
    *a = 0;
    for (int i = 0; i < 100; i++) {
        int pid = fork();
        if (pid == 0) {
            int *b = shmat(shm, 0, 0);
            while (true) {
                sem_wait(&sem_id);
                *b = *b + 1;
                if (*b > 10000) {
                    sem_post(&sem_id);
                    break;
                }
                printf("pid:%d,%d\n", getpid(), *b);
                sem_post(&sem_id);
            }
            return 0;
        }
    }
    sleep(4);
    sem_destroy(&sem_id);
    shmctl(shm, IPC_CREAT, 0);
    return 0;
}
