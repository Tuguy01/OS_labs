#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* start_routine(void* param)
{
    char** arr = (char**)param;
    for (int i = 0; ; i++)
    {
        if (arr[i] == NULL)
            return NULL;
        printf("%s\n", arr[i]);
    }
}

int main(int argc, char* argv[])
{
    pthread_t threads[4];

    char* arg1[] = {"thread1-1", "thread1-2", "thread1-3", "thread1-4", NULL};
    char* arg2[] = {"thread2-1", "thread2-2", "thread2-3", NULL};
    char* arg3[] = {"thread3-1", "thread3-2", NULL};
    char* arg4[] = {"thread4-1", NULL};

    char** argarr[] = {arg1, arg2, arg3, arg4};

    int create_error;
    int join_error;

    for (int i = 0; i < 4; i++)
    {
        create_error = pthread_create(&threads[i], NULL, start_routine, (void*)argarr[i]);
        if (create_error != 0)
        {
            perror("pthread_create error");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        join_error = pthread_join(threads[i], NULL);
        if (join_error != 0)
        {
            perror("pthread_join error");
            return 2;
        }
    }

    pthread_exit(NULL);

}

