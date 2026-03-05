#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _Line_ {
    struct _Line_* next;
    char* text;
} Line;

typedef struct _Headers_ {
    int stdio, string, stdlib;
} Headers;


FILE* checkFile(char* filename, char** new_name);
char* parseFile(FILE* file, Line** lines);
Line* addToList(Line* head, char* text);
FILE* writeOutput(Line* lines, char* filename);
int checkSubstrings(char* string, int num, ...);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Missing argument!\n");
        return 1;
    }

    char* new_name;
    FILE* input_file = checkFile(argv[1], &new_name);
    if (!input_file)
    {
        return 1;
    }

    Line* lines = NULL;
    char* file_raw = parseFile(input_file, &lines);
    if (!file_raw)
    {
        return 1;
    }

    strcat(new_name, ".c");
    FILE* output_file = writeOutput(lines, new_name);


    free(file_raw);
    free(new_name);
    fclose(input_file);
    fclose(output_file);
    return 0;
}

FILE* checkFile(char* filename, char** new_name)
{
    char* copy = calloc(1, strlen(filename) + 1); // +1 for '\0'
    if (copy == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }
    strncpy(copy, filename, strlen(filename));

    char* ptr;
    char last[100];

    ptr = strtok(copy, ".");
    
    *new_name = calloc(1, strlen(ptr) + 1);
    if (*new_name == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }
    strncpy(*new_name, ptr, strlen(ptr));

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

char* parseFile(FILE* file, Line** lines)
{
    int raw_size = 100;
    char* raw = calloc(1, raw_size);
    if (raw == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }

    char ch;
    int index = 0;
    int line_size = 100;
    char* new_line = calloc(1, line_size);
    if (new_line == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }

    while ((ch = fgetc(file)) != EOF)
    {
        if (index >= raw_size)
        {
            raw_size += raw_size;
            char* tmp = realloc(raw, raw_size);
            if (tmp == NULL)
            {
                printf("Memory error!\n");
                return NULL;
            }
            raw = tmp;

            
        }
        if (index >= line_size)
        {
            line_size += line_size;
            char* tmp = realloc(new_line, line_size);
            if (tmp == NULL)
            {
                printf("Memory error!\n");
                return NULL;
            }
            new_line = tmp;
        }

        if (ch == '\n')
        {
            *lines = addToList(*lines, new_line);
            new_line[0] = '\0';
        }
        else
        {
            strncat(new_line, &ch, 1);
            raw[index] = ch;
            index++;
        }
    }
    printf("%s\n", new_line);
    free(new_line);
    return raw;
}

Line* addToList(Line* head, char* text)
{
    Line* new_line = malloc(sizeof(Line));
    if (new_line == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }

    char* new_text = calloc(1, strlen(text) + 1);
    if (new_text == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }
    strcpy(new_text, text);

    new_line->text = new_text;
    new_line->next = NULL;

    if (head == NULL)
    {
        return new_line;
    }

    Line* current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_line;
    return head;
}

FILE* writeOutput(Line* lines, char* filename)
{
    FILE* file = fopen(filename, "w");

    // stdio.h: flose, feof, ferror, fgetc, fgets, fopen, fprintf, fputc, fputs, fread, fscanf, fseek, ftell, fwrite, getc, getchar, printf, putc, 
    //          putchar, puts, remove, rename, rewind, scanf, snprintf, sscanf

    // int stdio = checkSubstrings(raw, 3, "fclose", "feof", "ferror");
    // printf("%d\n", stdio);

    Headers* headers = malloc(sizeof(Headers));
    if (headers == NULL)
    {
        printf("Memory error!\n");
        return NULL;
    }

    Line* current = lines;
    while (current != NULL)
    {
        char* current_line = current->text;
        //printf("%s\n", current_line);

        if (strstr(current_line, "print") && !headers->stdio)
        {
            fputs("#include <stdio.h>", file);
            headers->stdio = 1;
        }
        current = current->next;
    }

    // main start
    fputs("\nint main() \n{\n", file);


    



    // main end
    fputs("return 0;\n}", file);

    free(headers);
    return file;
}

int checkSubstrings(char* string, int num, ...)
{
    va_list args;
    va_start(args, num);

    for (int i = 0; i < num; i++)
    {
        if (strstr(string, va_arg(args, char*)))
        {
            return 1;
        }
    }

    va_end(args);
    return 0;
}
