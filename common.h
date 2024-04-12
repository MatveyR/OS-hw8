#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define BUF_SIZE 10

typedef struct {
    int store[BUF_SIZE];
    pid_t reader_pid;
    pid_t writer_pid;
    int have_reader;
    // int have_writer;
} shared_memory;

extern const char *shar_object;
extern int buf_id;
extern shared_memory *buffer;
extern sem_t *mutex, *empty, *full, *admin;
extern const char *mutex_sem_name, *empty_sem_name, *full_sem_name, *admin_sem_name;

extern const char *reader_count_sem_name;
extern sem_t *reader_count_sem;   // семафор для подсчета числа активных читателей

extern const char *writer_sem_name;
extern sem_t *writer_sem;   // семафор для писателей

void init(void);
void close_common_semaphores(void);
void unlink_all(void);

#endif
