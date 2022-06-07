#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutexs[3];

void* start_routine(void* param)
{
    for (int i = 0; i < 10; i++)
    {

        pthread_mutex_lock(&mutexs[0]);
        pthread_mutex_lock(&mutexs[1]);
        printf("He is parrent!\n");
        pthread_mutex_unlock(&mutexs[1]);
        pthread_mutex_unlock(&mutexs[0]);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t new_thread;
    int create_error;

    for (int i = 0; i < 3; i++)
    {
        pthread_mutex_init(&mutexs[i], NULL);
    }

    create_error = pthread_create(&new_thread, NULL, start_routine, NULL);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutexs[1]);
        pthread_mutex_lock(&mutexs[0]);
        printf("I'm parent!\n");
        pthread_mutex_unlock(&mutexs[0]);
        pthread_mutex_unlock(&mutexs[1]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_mutex_destroy(&mutexs[i]);
    }
    pthread_exit(NULL);
}
