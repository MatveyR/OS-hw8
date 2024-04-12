#include "common.h"

const char *shar_object = "/shared-memory";

const char *mutex_sem_name = "/mutex-semaphore";
sem_t *mutex;

const char *empty_sem_name = "/empty-semaphore";
sem_t *empty;

const char *full_sem_name = "/full-semaphore";
sem_t *full;

const char *admin_sem_name = "/admin-semaphore";
sem_t *admin;

const char *reader_count_sem_name = "/reader-count-semaphore";
sem_t *reader_count_sem;

const char *writer_sem_name = "/writer-semaphore";
sem_t *writer_sem;

int buf_id;
shared_memory *buffer;

void init(void) {
    // Создание или открытие семафоров
    mutex = sem_open(mutex_sem_name, O_CREAT, 0666, 1);
    empty = sem_open(empty_sem_name, O_CREAT, 0666, BUF_SIZE);
    full = sem_open(full_sem_name, O_CREAT, 0666, 0);
    admin = sem_open(admin_sem_name, O_CREAT, 0666, 1);
    reader_count_sem = sem_open(reader_count_sem_name, O_CREAT, 0666, 0);
    writer_sem = sem_open(writer_sem_name, O_CREAT, 0666, 1);

    // Создание или открытие общего объекта разделяемой памяти
    buf_id = shm_open(shar_object, O_CREAT | O_RDWR, 0666);
    ftruncate(buf_id, sizeof(shared_memory));
    buffer = mmap(0, sizeof(shared_memory), PROT_WRITE | PROT_READ, MAP_SHARED, buf_id, 0);

    // Инициализация структуры разделяемой памяти
    for(int i = 0; i < BUF_SIZE; ++i) {
        buffer->store[i] = -1;
    }
    buffer->have_reader = 0;
}

void close_common_semaphores(void) {
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);
    sem_close(admin);
    sem_close(reader_count_sem);
    sem_close(writer_sem);
}

void unlink_all(void) {
    sem_unlink(mutex_sem_name);
    sem_unlink(empty_sem_name);
    sem_unlink(full_sem_name);
    sem_unlink(admin_sem_name);
    sem_unlink(reader_count_sem_name);
    sem_unlink(writer_sem_name);
    shm_unlink(shar_object);
}
