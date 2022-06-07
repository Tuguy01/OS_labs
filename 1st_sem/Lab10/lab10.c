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
    int execvp_result;

    if (argc < 2)
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
        execvp_result = execvp(argv[1], &argv[1]);
        if (execvp_result == -1)
        {
            perror("Execvp error");
            return -1;
        }
    }

    wait_result = wait(&current_state);
    if (wait_result == -1)
    {
        perror("Wait error");
        return -1;
    }

    printf("Exit status: %d\n", WEXITSTATUS(current_state));
    return WEXITSTATUS(current_state);
}

