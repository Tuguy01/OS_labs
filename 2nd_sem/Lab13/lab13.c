#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_cond_t cond;
pthread_mutex_t mutex;
int print_time = 1;

void* start_routine(void* param)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while (print_time == 1)
            pthread_cond_wait(&cond, &mutex);
        printf("He is parrent!\n");
        print_time = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t new_thread;
    int create_error;

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    create_error = pthread_create(&new_thread, NULL, start_routine, NULL);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while (print_time == 0)
            pthread_cond_wait(&cond, &mutex);
        printf("I'm parent!\n");
        print_time = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}

