#include "common.h"
#include <signal.h>

void sigfunc(int sig) {
    if(sig == SIGINT || sig == SIGTERM) {
        printf("Writer process received SIGINT/SIGTERM\n");
        sem_close(writer_sem);
        sem_unlink(writer_sem_name);
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
        // Проверка количества активных читателей
        while (1) {
            if (sem_trywait(reader_count_sem) == 0) {
                // Если активных читателей нет
                if(sem_post(reader_count_sem) == -1) {
                    perror("sem_post: Incorrect post of reader count semaphore");
                    exit(-1);
                }
                break;
            }
            sleep(1); // Ждём активных читателей
        }

        // Критическая точка
        if(sem_wait(writer_sem) == -1) {
            perror("sem_wait: Incorrect wait of writer semaphore");
            exit(-1);
        }

        // Запись в буффер
        buffer->store[buffer->writer_pid % BUF_SIZE] = rand() % 11;
        printf("Writer %d: Writes value = %d to cell [%d]\n",
               getpid(), buffer->store[buffer->writer_pid % BUF_SIZE], buffer->writer_pid % BUF_SIZE);

        // Критическая точка
        if(sem_post(writer_sem) == -1) {
            perror("sem_post: Incorrect post of writer semaphore");
            exit(-1);
        }

        // Sleep for random time
        sleep(3);
    }

    return 0;
}
