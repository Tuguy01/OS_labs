#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* start_routine(void* param)
{
    for (int i = 0; i < 10; i++)
        printf("He's parent!\n");
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t new_thread;
    int create_error;
    int join_error;
    void** return_value;

    create_error = pthread_create(&new_thread, NULL, start_routine, NULL);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    join_error = pthread_join(new_thread, return_value);
    if (join_error != 0)
    {
        perror("pthread_join error");
        return 2;
    }

    for (int i = 0; i < 10; i++)
        printf("I'm parent!\n");

    pthread_exit(NULL);
}

