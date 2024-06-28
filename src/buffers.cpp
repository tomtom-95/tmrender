#ifndef BUFFERS_CPP
#define BUFFERS_CPP

struct Buffer
{
    char *data;
    size_t count;
};

struct Point3D
{
    double x;
    double y;
    double z;
};

struct Point3DBuffer
{
    struct Point3D *data;
    size_t count;
};

struct TriangleIndices
{
    int idx0;
    int idx1;
    int idx2;
};

struct TriangleIndicesBuffer
{
    struct TriangleIndices *data;
    size_t count;
};


struct Buffer
BufferAllocate(size_t count);

void
BufferFree(struct Buffer *buffer);

struct Point3DBuffer
Point3DBufferAllocate(size_t count);

void
Point3DBufferFree(struct Point3DBuffer *buffer);

struct TriangleIndicesBuffer
TriangleIndicesBufferAlloc(size_t count);

void
TriangleIndicesBufferFree(struct TriangleIndicesBuffer *buffer);

struct Buffer
ReadEntireFile(const char *filename);

// size_t
// read_line_from_buffer(struct Buffer buffer,
//                       size_t buffer_offset);


struct Buffer
BufferAllocate(size_t count)
{
    struct Buffer buffer = {};

    buffer.data = (char *)malloc(count);
    if (buffer.data)
    {
        buffer.count = count;
    }
    else
    {
        print_error(__FILE__, __LINE__, "malloc error");
    }

    return buffer;
}

void
BufferFree(struct Buffer *buffer)
{
    if (buffer -> data)
    {
        free(buffer -> data);
    }
    *buffer = {};
}

struct Point3DBuffer
Point3DBufferAllocate(size_t count)
{
    struct Point3DBuffer buffer = {};

    buffer.data = (struct Point3D *)malloc(sizeof(*(buffer.data)) * count);
    if (buffer.data)
    {
        buffer.count = count;
    }
    else
    {
        print_error(__FILE__, __LINE__, "malloc failed");
    }

    return buffer;
}

void
Point3DBufferFree(struct Point3DBuffer *buffer)
{
    if (buffer -> data)
    {
        free(buffer -> data);
    }
    *buffer = {};
}


struct TriangleIndicesBuffer
TriangleIndicesBufferAlloc(size_t count)
{
    struct TriangleIndicesBuffer buffer = {};

    buffer.data = (struct TriangleIndices *)malloc(sizeof(*(buffer.data)) * count);
    if (buffer.data)
    {
        buffer.count = count;
    }
    else
    {
        print_error(__FILE__, __LINE__, "malloc failed");
    }

    return buffer;
}

void
TriangleIndicesBufferFree(struct TriangleIndicesBuffer *buffer)
{
    if (buffer -> data)
    {
        free(buffer -> data);
    }
    *buffer = {};
}

struct Buffer
ReadEntireFile(const char *filename)
{
    struct Buffer buffer = {};
        
    FILE *file = fopen(filename, "rb");
    if (file)
    {
#if _WIN32
        struct __stat64 stat;
        _stat64(filename, &stat);
#else
        struct stat stat;
        stat(filename, &stat);
#endif
        buffer = BufferAllocate(stat.st_size);
        if (buffer.data)
        {
            if (fread(buffer.data, buffer.count, 1, file) != 1)
            {
                print_error(__FILE__, __LINE__, "fread failed");
                BufferFree(&buffer);
            }
        }
        
        fclose(file);
    }
    else
    {
        print_error(__FILE__, __LINE__, "fopen failed");
    }
    
    return buffer;
}

// void swap_vertices(struct Vertex *v0, struct Vertex *v1)
// {
//     struct Vertex tmp = *v0;
//     *v0 = *v1;
//     *v1 = tmp;
// }
// 
// size_t
// read_line_from_buffer(struct Buffer buffer,
//                       size_t buffer_offset)
// {
//     for (;;)
//     {
//         if (buffer_offset + 1 == buffer.size)
//         {
//             return buffer_offset + 1;
//         }
// #if _WIN32
//         else
//         {
//             if (buffer.start[buffer_offset] == '\r' &&
//                 buffer.start[buffer_offset + 1] == '\n')
//             {
//                 return buffer_offset + 2;
//             }
//             else
//             {
//                 buffer_offset++;
//             }
//         }
// #else
//         else
//         {
//             if (buffer.start[buffer_offset] == '\n')
//             {
//                 return buffer_offset + 1;
//             }
//             else
//             {
//                 buffer_offset++;
//             }
//         }
// #endif
//     }
// }

#endif // #ifndef BUFFER_CPP