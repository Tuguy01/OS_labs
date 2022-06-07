#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("Real user ID is %d\n", getuid());
    printf("Effective user ID is %d\n", geteuid());

    if( argc < 1 )
    {
        perror("Wrong arguments\n");
        return 0;
    }

    FILE* file = fopen(argv[1], "r");
    if( file == NULL )
    {
        perror("Can't open file. ");
    }
    else
    {
        printf("File openned successfully.\n");
    }

    fclose(file);

    seteuid(getuid());
    printf("Real user ID is %d\n", getuid());
    printf("Effective user ID is %d\n", geteuid());

    file = fopen(argv[1], "r");
    if( file == NULL )
    {
        perror("Can't open file. ");
    }
    else
    {
        printf("File openned successfully.\n");
    }

    fclose(file);
    return 0;
}

