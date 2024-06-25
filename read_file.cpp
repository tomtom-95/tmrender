#include <stdlib.h>
#include <stdio.h>

#include "utils.cpp"
#include "buffers.cpp"

int main(int argc, char **argv)
{
    char *filename = "./obj/african_head/african_head_small.obj";
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error opening the file\n");
    }
    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    struct Buffer buffer = buffer_alloc(file_size + 1);
    if (buffer.start == NULL)
    {
        printf("cannot allocate buffer\n");
    }

    size_t bytes_read = fread(buffer.start,
                              sizeof(char),
                              file_size,
                              fp);
    if (bytes_read != file_size)
    {
        printf("error reading file\n");
    }
    buffer.start[file_size] = '\0';

    for (size_t i = 0; i < file_size + 1; i++)
    {
        printf("%c", buffer.start[i]);
    }

    return 0;
}