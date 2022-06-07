#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char filename[FILENAME_MAX];
    DIR *directory;

    directory = opendir(".");
    if (directory == NULL)
    {
         perror("Error with opendir\n");
         return -1;
    }

    printf("Enter filename: ");
    fgets(filename, FILENAME_MAX, stdin);
    if (filename == NULL)
    {
        perror("Error with fgets");
        return -1;
    }

    int name_length = strlen(filename) - 1;
    filename[name_length] = 0;

    for (int i = 0; i < name_length; i++)
    {
        if (filename[i] == '/')
        {
            printf("Sorry, but \"/\" symbol is not available in file name\n");
            return -1;
        }
    }

    struct dirent *current_file;
    int number_of_files = 0;

    while ((current_file = readdir(directory)) != NULL)
    {
        int current_length = strlen(current_file->d_name);
        int flag = 0;
        int i, j;
        for (i = 0, j = 0; (i < current_length) && (j < name_length); i++)
        {
            if ((filename[j] != '?') && (filename[j] != '*'))
            {
                if (filename[j] != current_file->d_name[i])
                    break;
                j++;
            }
            else if (filename[j] == '?')
                j++;
            else
            {
                while (j < name_length)
                {
                    j++;
                    if (filename[j] != '*')
                        break;
                }
                if (name_length == j)
                {
                    flag = 1;
                    break;
                }
                if (filename[j] == '?')
                {
                    j++;
                    continue;
                }
                while (i < current_length)
                {
                    if (filename[j] == current_file->d_name[i])
                        break;
                    i++;
                }
                j++;
            }
        }
        if (current_length == i)
        {
            while (j < name_length)
            {
                if (filename[j] != '*')
                    break;
                j++;
            }
            if (name_length == j)
                flag = 1;
        }
        if ((flag == 1) || (current_length == i && name_length == j))
        {
            printf("%s\n", current_file->d_name);
            number_of_files++;
        }
    }
    if (number_of_files == 0)
        printf("Nothing was found, filename: %s\n", filename);
    int closedir_result = closedir(directory);
    if (closedir_result == -1)
    {
        perror("Closedir error\n");
        return -1;
    }
    return 0;
}

