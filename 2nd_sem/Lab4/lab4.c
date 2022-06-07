#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* start_routine(void* param)
{
    int error_flag;
    while(1)
    {
        error_flag = write(STDIN_FILENO, "I'm writing something\n", 22);
        if (error_flag != 22)
        {
            perror("write_error");
            return NULL;
        }
    }
}

int main()
{
    pthread_t new_thread;
    int error_flag;
    error_flag = pthread_create(&new_thread, NULL, start_routine, NULL);
    if (error_flag != 0)
    {
        perror("pthread_create error");
        return 1;
    }
    sleep(2);

    error_flag = pthread_cancel(new_thread);
    if (error_flag != 0)
    {
        perror("pthread_cancel error");
        return 2;
    }

    error_flag = pthread_join(new_thread, NULL);
    if (error_flag != 0)
    {
        perror("pthread_join error");
        return 3;
    }
    return 0;
}

