#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE* checkFile(char* filename);
char* parseFile(FILE* file);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Missing argument!\n");
        return 1;
    }

    FILE* input_file = checkFile(argv[1]);
    if (!input_file)
    {
        return 1;
    }

    char* file_raw = parseFile(input_file);
    if (!file_raw)
    {
        return 1;
    }

    printf("%s\n", file_raw);

    //FILE* output_file = fopen(argv[1], "w");





    free(file_raw);
    fclose(input_file);
    //fclose(output_file);
    return 0;
}

FILE* checkFile(char* filename)
{
    char* copy = malloc(strlen(filename) + 1); // +1 for '\0'
    if (copy == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }
    strncpy(copy, filename, strlen(filename));

    char* ptr;
    char last[100];

    ptr = strtok(copy, ".");
    while (ptr != NULL)
    {
        ptr = strtok(NULL, ".");

        if (ptr != NULL)
        {
            strncpy(last, ptr, strlen(ptr));
        }
    }
    free(copy);

    if (strcmp(last, "oli"))
    {
        printf("Invalid file format!\n");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Invalid file!\n");
        return NULL;
    }
    return file;
}

char* parseFile(FILE* file)
{
    int raw_size = 100;
    char* raw = malloc(raw_size);
    if (raw == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }

    char ch;
    int index = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (index >= raw_size)
        {
            printf("Larger!\n");
            raw_size += raw_size;
            char* tmp = realloc(raw, raw_size);
            if (tmp == NULL)
            {
                printf("Memory error!\n");
                return NULL;
            }
            raw = tmp;
        }

        raw[index] = ch;
        index++;
    }
    return raw;
}
