#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <signal.h>

int end_flag = 0;

pthread_mutex_t mutex;

typedef struct info_for_thread
{
    int thread_number;
    int shift;
    double result;
}info_for_thread;

void* count(void* args)
{
    if (args == NULL)
    {
        perror("wrong thread arguments");
        return NULL;
    }
    info_for_thread* info = (info_for_thread*)args;
    double pi = 0.0;
    long i = info->thread_number;
    long thread_number = i;
    while(1)
    {
    	if ((i - thread_number) % (info->shift * 10000) == 0)
    	{
    		pthread_mutex_lock(&mutex);
    		if (end_flag != 0)
    		{
    			pthread_mutex_unlock(&mutex);
    			info->result = pi;
    			pthread_exit(NULL);
    		}
    		pthread_mutex_unlock(&mutex);
    	}
        pi += 1.0 / (i * 4.0 + 1.0);
        pi -= 1.0 / (i * 4.0 + 3.0);
        i += info->shift;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        perror("Wrong arguments");
        return 1;
    }
    int num_threads = atoi(argv[1]);
    if (errno == ERANGE || num_threads <= 0)
    {
        perror("Wrong number of threads");
        return 1;
    }

    sigset_t set;
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    pthread_mutex_init(&mutex, NULL);

    int error_flag;
    pthread_t sigthread;
    error_flag = pthread_create(&sigthread, NULL, sig_routine, NULL);
    if (error_flag != 0)
    {
    	perror("pthread_create error");
    	return 1;
    }


    double pi = 0;

    info_for_thread** info_arr = (info_for_thread**)malloc(sizeof(info_for_thread*) * num_threads);
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (threads == NULL || info_arr == NULL)
    {
        perror("can't allocate memory");
        return 1;
    }


    for (int i = 0; i < num_threads; i++)
    {
        info_arr[i] = (info_for_thread*)malloc(sizeof(info_for_thread) * num_threads);
        if (info_arr[i] == NULL)
        {
            perror("can't allocate memory");
            free(info_arr);
            free(threads);
            return 1;
        }
        info_arr[i]->thread_number = i;
        info_arr[i]->shift = num_threads;
        info_arr[i]->result = 0.0;

        error_flag = pthread_create(&(threads[i]), NULL, count, info_arr[i]);
        if (error_flag != 0)
        {
            perror("pthread_create error");
            for (int i = 0; i < num_threads; i++)
            {
            	free(info_arr[i]);
            }
            free(info_arr);
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        error_flag = pthread_join(threads[i], NULL);
        if (error_flag != 0)
        {
            perror("pthread_join error");
            for (int i = 0; i < num_threads; i++)
            {
            	free(info_arr[i]);
            }
            free(info_arr);
            free(threads);
            return 1;
        }
        pi += info_arr[i]->result;
    }

    error_flag = pthread_join(sigthread, NULL);
    if (error_flag != 0)
    {
    	perror("pthread_join error");
    	for (int i = 0; i < num_threads; i++)
        {
            free(info_arr[i]);
        }
        free(info_arr);
        free(threads);
        return 1;
    }
    
    pi = pi * 4.0;
    printf("PI = %lf\n", pi);

    for (int i = 0; i < num_threads; i++)
    {
        free(info_arr[i]);
    }
    free(info_arr);
    free(threads);
    return 0;
}
