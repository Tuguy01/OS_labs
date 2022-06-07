#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define DEFAULT_SIZE 10000

int create_table(off_t* offsets_table, off_t* lengths_of_lines, int file_descriptor)
{
    off_t text_size = 1;
    off_t number_of_line = 1;
    off_t number_of_offset = 1;
    off_t offset = 0;
    char* current_buffer = (char*)malloc(DEFAULT_SIZE * sizeof(char));
    if (current_buffer == NULL)
    {
        perror("Can't allocate memory\n");
        return -1;
    }

    while(text_size > 0)
    {
        text_size = read(file_descriptor, current_buffer, DEFAULT_SIZE);
        if (text_size == -1)
        {
            free(current_buffer);
            perror("Can't read file\n");
            free(current_buffer);
            return -1;
        }
        for(int i = 0; i < text_size; i++)
        {
            lengths_of_lines[number_of_line]++;
            offset++;
            if (current_buffer[i] == '\n')
            {
                offsets_table[number_of_line] = offset - lengths_of_lines[number_of_line];
                number_of_line++;
                number_of_offset++;
            }
        }
    }
    free(current_buffer);
    return number_of_line;
}


int search(int file_descriptor, off_t* offsets_table, off_t* lengths_of_lines, off_t number_of_lines)
{
    off_t line_number = 0;
    char* current_line = (char*)malloc(sizeof(char));
    char* console_in = (char*)calloc(BUFSIZ, sizeof(char));
    if (current_line == NULL || console_in == NULL )
    {
        perror("Can't allocate memory for cosole input\n");
        return -1;
    }

    int read_write_result = 0;
    int wrong_symbol = 0;

    while(1){
        read_write_result = write(STDOUT_FILENO, "Print number of line:\n", 22);
        if (read_write_result == -1)
        {
            perror("Can't print line for user\n");
            free(current_line);
            free(console_in);
            return -1;
        }

        read_write_result = read(STDIN_FILENO, console_in, BUFSIZ);
        if (read_write_result == -1)
        {
            perror("Reading error\n");
            continue;
        }

        for (int i = 0; i < read_write_result; i++)
        {
            if ((console_in[i] < '0' || console_in[i] > '9') &&  console_in[i] != '\n')
            {
                printf("Write only numbers\n\n");
                wrong_symbol = 1;
                break;
            }
        }

        if (wrong_symbol > 0)
        {
            wrong_symbol = 0;
            continue;
        }
        char* nullp = NULL;
        line_number = strtol(console_in, &nullp, 0);
        if (line_number < 0 || line_number > number_of_lines - 1)
        {
            printf("Incorrect line number\n\n");
            continue;
        }

        if (line_number == 0)
        {
            break;
        }

        if (lengths_of_lines[line_number] != 0)
        {
            char* tmp = NULL;
            tmp = (char*)realloc(tmp, lengths_of_lines[line_number] * sizeof(char));

            if (tmp == NULL){
                perror("Can't allocate memory for line\n");
                free(console_in);
                free(current_line);
                return -1;
            }

            current_line = tmp;

            off_t lseek_result = 0;
            lseek_result = lseek(file_descriptor, offsets_table[line_number], SEEK_SET);

            if ((int)lseek_result == -1)
            {
                perror("Error with lseek\n");
                free(console_in);
                free(current_line);
                return -1;
            }

            read_write_result = read(file_descriptor, current_line, lengths_of_lines[line_number]);

            if (read_write_result == -1)
            {
                perror("Error with reading\n");
                free(console_in);
                free(current_line);
                return -1;
            }


            read_write_result = write(STDOUT_FILENO, current_line, lengths_of_lines[line_number]);
            if (read_write_result == -1)
            {
                perror("Can't write line\n");
                free(console_in);
                free(current_line);
                return -1;
            }
        }

    }

    free(console_in);
    free(current_line);
    return 0;
}

int main(int argc, char* argv[])
{
    off_t* offsets_table = (off_t*)malloc(DEFAULT_SIZE * sizeof(off_t));
    off_t* lengths_of_lines = (off_t*)malloc(DEFAULT_SIZE * sizeof(off_t));

    if (offsets_table == NULL || lengths_of_lines == NULL)
    {
        perror("Can't allocate memory for tables\n");
        return -1;
    }
    off_t number_of_lines = 0;
    int file_descriptor = 0;


    if (argc != 2)
    {
        perror("Wrong arguments!\n");
        free(offsets_table);
        free(lengths_of_lines);
        return -1;
    }

    file_descriptor = open(argv[1], O_RDONLY);

    if (file_descriptor == -1)
    {
        perror("Can't open file!\n");
        free(offsets_table);
        free(lengths_of_lines);
        return -1;
    }

    number_of_lines = create_table(offsets_table, lengths_of_lines, file_descriptor);
    int close_result = 0;
    if (number_of_lines == -1)
    {
        close_result = close(file_descriptor);
        if (close_result == -1)
        {
            perror("Can't close file\n");
        }
        free(offsets_table);
        free(lengths_of_lines);
        return -1;
    }

    search(file_descriptor, offsets_table, lengths_of_lines, number_of_lines);

    close_result = close(file_descriptor);
    if (close_result == -1)
    {
        perror("Can't close file\n");
        free(offsets_table);
        free(lengths_of_lines);
        return -1;
    }
    free(offsets_table);
    free(lengths_of_lines);
    return 0;
}

