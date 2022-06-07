#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* exit_routine(void* param)
{
    write(STDIN_FILENO, "You shall not pass!\n", 20);
}

void* start_routine(void* param)
{
    int error_flag;
    pthread_cleanup_push(exit_routine, NULL);
    int i = 0;
    while(i < 10000)
    {
        i++;
        error_flag = write(STDIN_FILENO, "If I take one more step, it'll be the farthest away from home I've ever been.\n", 78);
        if (error_flag != 78)
        {
            perror("write_error");
            return NULL;
        }
    }
    pthread_cleanup_pop(0);
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

    /*error_flag = pthread_cancel(new_thread);
    if (error_flag != 0)
    {
        perror("pthread_cancel error");
        return 2;
    }*/

    error_flag = pthread_join(new_thread, NULL);
    if (error_flag != 0)
    {
        perror("pthread_join error");
        return 3;
    }
    return 0;
}

