#ifndef BUFFERS_CPP
#define BUFFERS_CPP

struct Buffer
{
    char *data;
    size_t count;
};

struct Vertex
{
    double x;
    double y;
    double z;
};

struct Vector3D
{
    double x;
    double y;
    double z;
};

struct TriangleBoundingBox
{
    int x_min;
    int x_max;
    int y_min;
    int y_max;
};

struct VertexBuffer
{
    struct Vertex *data;
    size_t count;
};

struct Face
{
    int vertex_indices[3];
    int texture_indices[3];
    int normal_indices[3];
};

struct FaceBuffer
{
    struct Face *data;
    size_t count;
};


struct Buffer
BufferAllocate(size_t count);

void
BufferFree(struct Buffer *buffer);

struct VertexBuffer
VertexBufferAllocate(size_t count);

void
VertexBufferFree(struct VertexBuffer *buffer);

struct FaceBuffer
FaceBufferAlloc(size_t count);

void
FaceBufferFree(struct FaceBuffer *buffer);

struct Buffer
ReadEntireFile(const char *filename);


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

struct VertexBuffer
VertexBufferAllocate(size_t count)
{
    struct VertexBuffer buffer = {};

    buffer.data = (struct Vertex *)malloc(sizeof(*(buffer.data)) * count);
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
VertexBufferFree(struct VertexBuffer *buffer)
{
    if (buffer -> data)
    {
        free(buffer -> data);
    }
    *buffer = {};
}


struct FaceBuffer
FaceBufferAlloc(size_t count)
{
    struct FaceBuffer buffer = {};

    buffer.data = (struct Face *)malloc(sizeof(*(buffer.data)) * count);
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
FaceBufferFree(struct FaceBuffer *buffer)
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


#endif // #ifndef BUFFER_CPP