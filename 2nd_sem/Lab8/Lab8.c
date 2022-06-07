#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define num_steps 1000

void* count(void* steps)
{
	int my_steps = *(int*)steps;
	int interval = ceil(num_steps / my_steps);
	double* result = (double*)malloc(sizeof(double));
	double temp_result;

	for (int i = 0; i < my_steps; i += interval)
	{
		temp_result += 1.0 / (i * 4.0 + 1.0);
		temp_result -= 1.0 / (i * 4.0 + 3.0);
	}

	result[1] = temp_result * 4.0;
	pthread_exit((void*)result);
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

	int iter_for_thread = num_steps / num_threads;
	int additional_iter = num_steps % num_threads;
	double pi = 0;

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
	if (threads == NULL)
	{
        perror("can't allocate memory");
        return 1;
	}

	int error_flag;

	for (int i = 0; i < num_threads; i++)
	{
		if (additional_iter > 0)
		{
			iter_for_thread++;
		    error_flag = pthread_create(&threads[i], NULL, count, (void*)iter_for_thread);
		    iter_for_thread--;
		    additional_iter--;
	    }
	    else
	    {
	    	error_flag = pthread_create(&threads[i], NULL, count, (void*)iter_for_thread);
	    }

	    if (error_flag != 0)
	    {
	    	perror("pthread_create error");
	    	return 1;
	    }
	}

	double* results = (double*)malloc(sizeof(double) * num_threads);
	void* result;

    for (int i = 0; i < num_threads; i++)
    {
    	error_flag = pthread_join(threads[i], result);
    	results[i] = *(double*)result;

    	if (error_flag != 0)
	    {
	    	perror("pthread_join error");
	    	return 1;
	    }

	    pi += results[i];
    }

    printf("PI = %lf", pi);
    return 0;
}