#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;
pthread_mutex_t third_mutex;

void* start_routine(void* param)
{
    pthread_mutex_lock(&third_mutex);
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&first_mutex);
        printf("He's parent!\n");
        pthread_mutex_unlock(&third_mutex);

        pthread_mutex_lock(&second_mutex);
        printf("He's parent!\n");
        pthread_mutex_unlock(&first_mutex);

        pthread_mutex_lock(&third_mutex);
        printf("He's parent!\n");
        pthread_mutex_unlock(&second_mutex);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t new_thread;
    int create_error;

    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);
    pthread_mutex_init(&third_mutex, NULL);

    pthread_mutex_lock(&first_mutex);

    create_error = pthread_create(&new_thread, NULL, start_routine, NULL);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&second_mutex);
        printf("I'm parent!\n");
        pthread_mutex_unlock(&first_mutex);

        pthread_mutex_lock(&third_mutex);
        printf("I'm parent!\n");
        pthread_mutex_unlock(&second_mutex);

        pthread_mutex_lock(&first_mutex);
        printf("I'm parent!\n");
        pthread_mutex_unlock(&third_mutex);
    }
    pthread_exit(NULL);
}

