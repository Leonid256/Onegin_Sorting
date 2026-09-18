#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
//-------------------------------------------------------------------------------------------------
int ReadFromFile(const char* name, char** index);
int WriteToFile(const char* name, const char** index, size_t count);
char *my_strdup(const char *s);
int Compare(const void * ptr_a, const void * ptr_b);

//-------------------------------------------------------------------------------------------------
const int BUFFER_SIZE = 10000;
const int MAX_LINES = 10000;

//-------------------------------------------------------------------------------------------------
int main()
{
    char* index[MAX_LINES] = {};
    size_t count = ReadFromFile("/Users/leonid/Documents/GitHub/mipt_course/Onegin/Onegin.txt", index);
        
    qsort(index, count, sizeof(char*), Compare);
    
    WriteToFile("/Users/leonid/Documents/GitHub/mipt_course/Onegin/Res_Onegin.txt", (const char**)index, count);

    for (size_t i = 0; i < count; i++)
        free(index[i]);

    return 1;
}
//-------------------------------------------------------------------------------------------------
int Compare(const void * ptr_a, const void * ptr_b)
{
    const char* pt_a = *(const char**)ptr_a;
    const char* pt_b = *(const char**)ptr_b;

    while (*pt_a && *pt_b)
    {
        if (!isalpha(*pt_a) || !isalpha(*pt_b))
        {
            pt_a++;
            pt_b++;
            continue;
        }

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
int ReadFromFile(const char* name, char** index)
{
    FILE* file = fopen (name, "r");
    int i = 0;

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
int WriteToFile(const char* name, const char** index, size_t count)
{
    FILE* file = fopen (name, "w");
    int i = 0;

    if (file == NULL)
        return -1;

    while (i < count)
    {
        //printf("%s\n", index[i]);
        fputs((index[i++]), file);
        //fprintf(file, "\n");
    }

    fclose(file);

    return i;
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

    printf("%s", ptr);
    return ptr;
}