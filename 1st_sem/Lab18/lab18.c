#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    char* mode = (char*)malloc(11 * sizeof(char));
    if (mode == NULL)
    {
        perror("Can't allocate memory for mode array\n");
        return -1;
        }
    struct stat file_info;

    for (int i = 1; i < argc; i++)
    {
        if(lstat(argv[i], &file_info) == -1)
        {
            perror("lstat error\n");
                    continue;
        }
                printf("File: %s\n", argv[i]);


                if (S_ISREG(file_info.st_mode))
                {
                    mode[0] = '-';
                }
                else if (S_ISDIR(file_info.st_mode))
                {
                    mode[0] = 'd';
                }
                else
                {
                    mode[0] = '?';
                }

                mode[1] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
                mode[2] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
                mode[3] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';

                mode[4] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
                mode[5] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
                mode[6] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';

                mode[7] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
                mode[8] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
                mode[9] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';

                mode[10] = '\0';

                printf("%s\t", mode);

                printf("%d\t", file_info.st_nlink);

                struct passwd* password = getpwuid(file_info.st_uid);
                if (password == NULL)
                {
                    printf("%d\t", file_info.st_uid);
                }
                else
                {
                    printf("%u\t", password->pw_uid);
                }

                struct group* group = getgrgid(file_info.st_gid);
                if (group == NULL)
                {
                    printf("%d\t", file_info.st_gid);
                }
                else
                {
                    printf("%s\t", group->gr_name);
                }


                if (mode[0] == '-')
                {
                    printf("%ld\t", file_info.st_size);
                }
                else
                {
                    printf("\t");
                }
                char* last_change = (char*)calloc(FILENAME_MAX, sizeof(char));
                if (last_change == NULL)
                {
                    perror("Can't allocate memory for last_change");
                        free(mode);
                        return -1;
                }

                strftime(last_change, FILENAME_MAX, "%H:%M %e %b %Y", localtime(&(file_info.st_ctime)));
                printf("%s\t", last_change);
                free(last_change);
                printf("%s\n", basename(argv[i]));

    }
        free(mode);
        return 0;
}
