#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* start_routine(void* param)
{   
    char** arr = (char**)param;
    for (int i = 0; ; i++)
    {
        if (arr[i] == NULL)
            return (void*)i;
        printf("%s\n", arr[i]);
    }
}

int main(int argc, char* argv[])
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    char* arg1[] = {"thread1-1", "thread1-2", "thread1-3", "thread1-4", NULL};
    char* arg2[] = {"thread2-1", "thread2-2", "thread2-3", NULL};
    char* arg3[] = {"thread3-1", "thread3-2", NULL};
    char* arg4[] = {"thread4-1", NULL};

    int create_error;

    create_error = pthread_create(&thread1, NULL, start_routine, (void*)arg1);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    create_error = pthread_create(&thread2, NULL, start_routine, (void*)arg2);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    create_error = pthread_create(&thread3, NULL, start_routine, (void*)arg3);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    create_error = pthread_create(&thread4, NULL, start_routine, (void*)arg4);
    if (create_error != 0)
    {
        perror("pthread_create error");
        return 1;
    }

    pthread_exit(NULL);

    
}
