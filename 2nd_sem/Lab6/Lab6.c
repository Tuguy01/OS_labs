#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <math.h>

#define COEF 5

typedef struct
{
    char* string;
    int str_lenght;
}info;

void* sorter(void* args)
{
    info* string_info = (info*)args;
    sleep(string_info->str_lenght * COEF);
    printf("%s\n", string_info->string);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Wrong arguments");
        return -1;
    }

    info** strings = (info**)malloc((argc -1) * sizeof(info*));
    if (strings == NULL)
    {
        perror("can't allocate memory");
        return -2;
    }

    for (int i = 0; i < argc - 1; i++)
    {
        strings[i] = (info*)malloc(sizeof(info));
        if (strings[i] == NULL)
        {
            perror("can't allocate memory");
            return -2;
        }
        strings[i]->string = argv[i + 1];
        strings[i]->str_lenght = strlen(argv[i + 1]);
    }

    pthread_t* threads = (pthread_t*)malloc((argc - 1) * sizeof(pthread_t));
    if (threads == NULL)
    {
        perror("can't allocate memory");
        return -2;
    }
    int error_flag;
    for(int i = 0; i < argc - 1; i++)
    {
        error_flag = pthread_create(&threads[i], NULL, sorter, strings[i]);
        if(error_flag != 0)
            perror("Failed to create thread\n");
    }

    for(int i = 0; i < argc - 1; i++)
    {
        error_flag = pthread_join(threads[i], NULL);
        if(error_flag)
            perror("Failed to join thread\n");
    }
    
    return 0;
}   