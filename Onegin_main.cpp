#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
//-------------------------------------------------------------------------------------------------
int ReadFromFile(const char* name, char** index);
int WriteToFile(const char* name, const char** index, size_t count, const char* mode);
int WriteSpaces(const char* name, size_t val);
char *my_strdup(const char *s);
int Compare_Straight(const void * ptr_a, const void * ptr_b);
int Compare_Reverse(const void * ptr_a, const void * ptr_b);
int Compare_to_Initial(const void * ptr_a, const void * ptr_b);
int QuickSort(void* list, size_t num, size_t size, int (*compare)(const void *, const void *));
int ChangeValues(char* data, size_t value1, size_t value2, size_t size);
char *my_strrev(char *str);

//-------------------------------------------------------------------------------------------------
const int BUFFER_SIZE = 1000;
const int MAX_LINES = 10000;
const int SPACE = 10;
const char FILE_IN[] = "/Users/leonid/Documents/GitHub/mipt_course/Onegin/Onegin.txt";
const char FILE_OUT[] = "/Users/leonid/Documents/GitHub/mipt_course/Onegin/Res_Onegin.txt";

//TODO: убрать пустые строки в тексте
//-------------------------------------------------------------------------------------------------
int main()
{
    //strait sorting//
    char* index[MAX_LINES] = {};
    size_t count = ReadFromFile(FILE_IN, index);
        
    qsort(index, count, sizeof(char*), Compare_Straight);
    WriteToFile(FILE_OUT, (const char**)index, count, "w");
    
    WriteSpaces(FILE_OUT, SPACE);
    
    
    // reverse sorting//
    QuickSort(index, count, sizeof(char*), Compare_Reverse);
    WriteToFile(FILE_OUT, (const char**)index, count, "a");

    WriteSpaces(FILE_OUT, SPACE);

    //initial text//
    qsort(index, count, sizeof(char*), Compare_to_Initial);
    WriteToFile(FILE_OUT, (const char**)index, count, "a");

    //free buffer//
    for (size_t i = 0; i < count; i++)
    {
        index[i] = NULL;
        free(index[i]);
    }

    return 0;
}
//-------------------------------------------------------------------------------------------------
int Compare_Straight(const void * ptr_a, const void * ptr_b)
{
    const char* pt_a = *(const char**)ptr_a;
    const char* pt_b = *(const char**)ptr_b;

    while (*pt_a && *pt_b)
    {
        while (!isalpha(*pt_a))
            pt_a++;
        while (!isalpha(*pt_b))
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
int Compare_Reverse(const void * ptr_a, const void * ptr_b)
{
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    const char* start_a = *(const char**)(ptr_a);
    const char* start_b = *(const char**)(ptr_b);

    if (*start_a == '\0' || *start_b == '\0')
        return (*start_a != '\0') - (*start_b != '\0');

    size_t i_a = strlen(start_a) - 1;
    size_t i_b = strlen(start_b) - 1;

    while (i_a >= 0 && i_b >= 0)
    {
        while (i_a >= 0 && !isalpha(start_a[i_a]))
            i_a--;
        while (i_b >= 0 && !isalpha(start_b[i_b]))
            i_b--;

        if (i_a < 0 || i_b < 0)
            break;

        char a = tolower((char) start_a[i_a]);
        char b = tolower((char) start_b[i_b]);

        if (a != b)
            return (a - b);

        i_a--;
        i_b--;
    }

    return i_a - i_b;
}
//-------------------------------------------------------------------------------------------------
int Compare_to_Initial(const void * ptr_a, const void * ptr_b)
{
    const char** pt_a = (const char**)ptr_a;
    const char** pt_b = (const char**)ptr_b;

    return pt_a - pt_b;
}
//-------------------------------------------------------------------------------------------------
int ReadFromFile(const char* name, char** index)
{
    FILE* file = fopen (name, "r");
    size_t i = 0;

    if (file == NULL)
        return -1;

    while (!feof(file))
    {
        char buffer[BUFFER_SIZE] = "";

        fgets(buffer, BUFFER_SIZE, file);
        index[i++] = my_strdup(buffer);
    }

    fclose(file);

    return i;
}

//-------------------------------------------------------------------------------------------------
int WriteToFile(const char* name, const char** index, size_t count, const char* mode)
{
    FILE* file = fopen (name, mode);
    size_t i = 0;

    if (file == NULL)
        return -1;

    while (i < count)
    {
        //printf("%s\n", index[i]);
        fputs((index[i++]), file);
        //fprintf(file, "\n");
    }

    fclose(file);

    return i + 1;
}
//-------------------------------------------------------------------------------------------------
int WriteSpaces(const char* name, size_t val)
{
    FILE* file = fopen(name, "a");
    size_t i = 0;

    if (file == NULL)
        return -1;
    
    while (i < val)
    {
        fputs("\n", file);
        i++;
    }

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

    //printf("%s", ptr);
    return ptr;
}
//---------------------------------------------------------------------------------
int QuickSort(void* list, size_t num, size_t size, int (*compare)(const void *, const void *))
{
    if (num < 1)
        return 0;

    char* data = (char*) list;

    size_t last = 0;

    ChangeValues(data, (last), (num / 2), size);

    for (size_t i = 1; i < num; i++)
    {
        const void* pt_pivot = (const void*)(data + (0) * size);
        const void* pt_i = (const void*)(data + i * size);

        if ((*compare)(pt_pivot, pt_i) > 0)
        {
            last++;
            ChangeValues(data, (last), (i), size);
        }
    }

    ChangeValues(data, (last), (0), size);

    QuickSort((void*)(data), last, size, compare);
    QuickSort((void*)(data + (last + 1) * size), num - last - 1, size, compare);

    return 0;
}
//---------------------------------------------------------------------------------
int ChangeValues(char* data, size_t value1, size_t value2, size_t size)
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
    for (int i = 0; i <= len / 2; i++)
    {
        char t = str[i];
        str[i] = str[len - i];
        str[len - i] = t;
    }

    return &str[0];
}