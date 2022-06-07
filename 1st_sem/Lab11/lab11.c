#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int execvpe(char* file_name, char* argv[], char* envp[])
{
    int execvp_result = 0;
    extern char** environ;
    char** old_environ;
    old_environ = environ;
    environ = envp;
    execvp_result = execvp(file_name, argv);
    if (execvp_result == -1)
    {
        environ = old_environ;
        perror("execvp error\n");
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[], char* envp[])
{
    extern char** environ;
    char** new_env = (char**)malloc(19 * sizeof(char*));
    for (int i = 0; i < 17; i++)
    {
        new_env[i] = environ[i];
    }
    new_env[17] = "ANSWER=42";
    new_env[18] = NULL;
    return execvpe(argv[1], &argv[1], new_env);
}

