#include "common.h"
#include <signal.h>

void sigfunc(int sig) {
    if(sig == SIGINT || sig == SIGTERM) {
        printf("Reader process received SIGINT/SIGTERM\n");
        sem_post(admin);
        sem_close(reader_count_sem);
        sem_unlink(reader_count_sem_name);
        close_common_semaphores();
        unlink_all();
        exit(0);
    }
}

int main() {
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);

    srand(time(0));
    init();

    while (1) {
        // Увеличиваем число активных читателей
        if(sem_post(reader_count_sem) == -1) {
            perror("sem_post: Incorrect post of reader count semaphore");
            exit(-1);
        }

        // Критическая точка
        if(sem_wait(mutex) == -1) {
            perror("sem_wait: Incorrect wait of mutex semaphore");
            exit(-1);
        }

        // Чтение из буффера
        int result = buffer->store[buffer->reader_pid % BUF_SIZE];
        printf("Reader %d: Reads value = %d from cell [%d]\n",
               getpid(), result, buffer->reader_pid % BUF_SIZE);

        // Критическая точка
        if(sem_post(mutex) == -1) {
            perror("sem_post: Incorrect post of mutex semaphore");
            exit(-1);
        }

        sleep(3);
    }

    return 0;
}
