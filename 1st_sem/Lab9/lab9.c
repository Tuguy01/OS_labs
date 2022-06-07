#include <sys/types.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t fork_result;
    pid_t wait_result;
    int current_state;
    int execl_result;

    if (argc != 2)
    {
        perror("Wrong arguments");
        return -1;
    }

    fork_result = fork();
    if (fork_result == -1)
    {
        perror("Fork error");
        return -1;
    }

    if (fork_result == 0)
    {
        printf("\nNew proc\npid: %d\nppid: %d\n\n", getpid(), getppid());
        sleep(20);
        execl_result = execl("/bin/cat", "cat", argv[1], NULL);
        if (execl_result == -1)
        {
            perror("Cat error");
            return -1;
        }
    }

    printf("I am parrent\nChild's pid: %d\nMy pid: %d\n", fork_result, getpid());
    sleep(60);

    wait_result = wait(&current_state);
    if (wait_result == -1)
    {
        perror("Wait error");
        return -1;
    }

    printf("\nEverything is fine\n");
    return 0;
}

