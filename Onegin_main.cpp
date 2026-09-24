#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>

//-------------------------------------------------------------------------------------------------
struct string_index 
{
    char* string;
    size_t len;
};

//-------------------------------------------------------------------------------------------------
int read_file(const char* name, char* big_buffer, size_t count);
size_t add_to_index(string_index* index, char* big_buffer, size_t count);
int write_to_file(const char* name, string_index* index, size_t count, const char* mode);
int write_spaces(const char* name, size_t val);
char *my_strdup(const char *s);
int compare_straight(const void * ptr_a, const void * ptr_b);
int compare_reverse(const void * ptr_a, const void * ptr_b);
int compare_to_initial(const void * ptr_a, const void * ptr_b);
int quick_sort(void* list, size_t num, size_t size, int (*compare)(const void *, const void *));
int change_values(char* data, size_t value1, size_t value2, size_t size);
char *my_strrev(char *str);
void toxic_free(void* buffer);
size_t len_of_file(const char* name);

//-------------------------------------------------------------------------------------------------
const size_t   BUFFER_SIZE    = 1000;
const size_t   SPACE          = 10;
const char     FILE_IN[]      = "Onegin/Onegin.txt";
const char     FILE_OUT[]     = "Onegin/Res_Onegin.txt";
const int      POISON_VALUE   = 13;
const char     RES_DIVISION[] = "---------------------------------------------------------------------------------------------------------------------------------------";

//-------------------------------------------------------------------------------------------------
int main()
{
    size_t file_len = len_of_file(FILE_IN);
    size_t count = file_len / sizeof(char);

    //buffer for fread//
    char* big_buffer = (char*) calloc(count + 1, sizeof(char));
    if (big_buffer == NULL)
        return 1;
    size_t strings = read_file(FILE_IN, big_buffer, count);
    if (strings <= 0)
    {
        toxic_free(big_buffer);
        return 1;
    }

    string_index* index = (string_index*)calloc(strings, sizeof(string_index));
    if (index == NULL)
    {
        toxic_free(big_buffer);
        return 1;
    }
    count = add_to_index((string_index*)index, (char*)big_buffer, count);

    //strait sorting//
    qsort(index, count, sizeof(string_index), compare_straight);
    write_to_file(FILE_OUT, (string_index*)index, count, "w");
    
    write_spaces(FILE_OUT, SPACE);
    
    //reverse sorting//
    quick_sort(index, count, sizeof(string_index), compare_reverse);
    write_to_file(FILE_OUT, (string_index*)index, count, "a");

    write_spaces(FILE_OUT, SPACE);

    //initial text//
    qsort(index, count, sizeof(string_index), compare_to_initial);
    write_to_file(FILE_OUT, (string_index*)index, count, "a");

    //free buffer//
    toxic_free(index);
    toxic_free(big_buffer);

    return 0;
}
//-------------------------------------------------------------------------------------------------
int compare_straight(const void * ptr_a, const void * ptr_b)
{
    const char* pt_a = ((const string_index*)ptr_a) -> string;
    const char* pt_b = ((const string_index*)ptr_b) -> string;

    while (*pt_a && *pt_b)
    {
        while (*pt_a && !isalpha(*pt_a))
            pt_a++;
        while (*pt_b && !isalpha(*pt_b))
            pt_b++;

        char a = tolower((char) *pt_a);
        char b = tolower((char) *pt_b);

        if (a != b)
            return (a - b);

        pt_a++;
        pt_b++;
    }

    return (tolower((char) *pt_a) - tolower((char) *pt_b));
}
//-------------------------------------------------------------------------------------------------
int compare_reverse(const void * ptr_a, const void * ptr_b)
{
    if (ptr_a == NULL || ptr_b == NULL)
        return (ptr_a != NULL) - (ptr_b != NULL);

    const string_index* str_a = (const string_index*)ptr_a;
    const string_index* str_b = (const string_index*)ptr_b;

    ssize_t i_a = (ssize_t)str_a -> len - 1;
    ssize_t i_b = (ssize_t)str_b -> len - 1;

    while (i_a >= 0 && i_b >= 0)
    {
        while (i_a >= 0 && !isalpha((unsigned char)str_a -> string[i_a]))
            i_a--;
        while (i_b >= 0 && !isalpha((unsigned char)str_b -> string[i_b]))
            i_b--;

        if (i_a < 0 || i_b < 0)
            break;

        char a = (char)tolower((unsigned char) str_a -> string[i_a]);
        char b = (char)tolower((unsigned char) str_b -> string[i_b]);

        if (a != b)
            return (a - b);

        i_a--;
        i_b--;
    }

    if (i_a >= 0 && i_b < 0)
        return 1;
    else if (i_a < 0 && i_b >= 0)
        return -1;

    if (str_a -> len > str_b -> len)
        return 1;
    else if (str_a -> len < str_b -> len)
        return -1;

    return 0;
}
//-------------------------------------------------------------------------------------------------
int compare_to_initial(const void * ptr_a, const void * ptr_b)
{
    const string_index* pt_a = (const string_index*)ptr_a;
    const string_index* pt_b = (const string_index*)ptr_b;

    char* str_a = pt_a -> string;
    char* str_b = pt_b -> string;

    if (str_a > str_b)
        return 1;
    if (str_a < str_b)
        return -1;
    return 0;
}
//-------------------------------------------------------------------------------------------------
int read_file(const char* name, char* big_buffer, size_t count)
{
    FILE* file = fopen (name, "r");
    if (file == NULL)
        return -1;
    
    size_t strings = 0;
    size_t file_len = fread((void*) big_buffer, sizeof(char), count, file);

    fclose(file);

    char* buffer = (char*) big_buffer;
    for (size_t i = 0; i < file_len; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            strings++;
        }
    }

    return strings;
}
//-------------------------------------------------------------------------------------------------
size_t add_to_index(string_index* index, char* big_buffer, size_t count)
{
    if (index == NULL || big_buffer == NULL || count == 0)
        return -1;

    size_t i_buf = 0, i_ind = 0;

    while (i_buf < count)
    {
        while (i_buf < count && big_buffer[i_buf] == '\0')
            i_buf++;

        if (i_buf >= count)
            break;

        index[i_ind].string = &big_buffer[i_buf];
        size_t start_len = i_buf;

        while (i_buf < count && big_buffer[i_buf] != '\0')
            i_buf++;

        index[i_ind].len = i_buf - start_len;
        i_ind++;
    }
    
    return i_ind;
}

//-------------------------------------------------------------------------------------------------
int write_to_file(const char* name, string_index* index, size_t count, const char* mode)
{
    FILE* file = fopen (name, mode);
    if (file == NULL)
        return -1;
    
    size_t i = 0;

    while (i < count)
    {
        fputs((index[i++].string), file);
        fprintf(file, "\n");
    }

    fclose(file);

    return i + 1;
}
//-------------------------------------------------------------------------------------------------
int write_spaces(const char* name, size_t val)
{
    FILE* file = fopen(name, "a");
    if (file == NULL)
        return -1;

    size_t i = 0;
    
    fprintf(file, "%s\n", RES_DIVISION);
    while (i < val)
    {
        fputs("\n", file);
        i++;
    }
    fprintf(file, "%s\n", RES_DIVISION);

    fclose(file);

    return i + 1;
}
//-------------------------------------------------------------------------------------------------
char *my_strdup(const char *s)
{
    char* ptr = (char*) malloc(sizeof(char) * (strlen(s) + 1));

    int i = 0;
    for (; s[i] != '\0'; i++)
    {
        ptr[i] = s[i];
    }
    ptr[i] = s[i];

    return ptr;
}
//---------------------------------------------------------------------------------
int quick_sort(void* list, size_t num, size_t size, int (*compare)(const void *, const void *))
{
    if (num <= 1)
        return 0;

    char* data = ((char*) list);

    size_t last = 0;

    change_values(data, last, (num / 2), size);

    for (size_t i = 1; i < num; i++)
    {
        const void* pt_pivot = (const void*)(data + (0) * size);
        const void* pt_i = (const void*)(data + i * size);

        if ((*compare)(pt_pivot, pt_i) > 0)
        {
            last++;
            change_values(data, last, i, size);
        }
    }

    change_values(data, last, 0, size);

    quick_sort((void*)(data), last, size, compare);
    quick_sort((void*)(data + (last + 1) * size), num - last - 1, size, compare);

    return 0;
}
//---------------------------------------------------------------------------------
int change_values(char* data, size_t value1, size_t value2, size_t size)
{
    if (value1 == value2)
        return 0;

    for (size_t delta = 0; delta < size; delta++)
    {
        char temp = *(data + value2 * size + delta);
        *(data + value2 * size + delta) = *(data + value1 * size + delta);
        *(data + value1 * size + delta) = temp;
    }

    return 0;
}
//---------------------------------------------------------------------------------
char *my_strrev(char *str)
{
    size_t len = strlen(str) - 1;
    for (size_t i = 0; i <= len / 2; i++)
    {
        char t = str[i];
        str[i] = str[len - i];
        str[len - i] = t;
    }

    return &str[0];
}
//---------------------------------------------------------------------------------
void toxic_free(void* in_buffer)
{
    char* buffer = (char*) in_buffer;
    free(buffer);
    *buffer = POISON_VALUE;
}
//---------------------------------------------------------------------------------
size_t len_of_file(const char* name)
{
    struct stat file_info;
    stat(name, &file_info);

    return file_info.st_size;
}