#line 1 "buffers.cpp"



struct Buffer {
        char *start;
        size_t size;
};

struct Vertex {
        double a;
        double b;
        double c;
};

struct VertexBuffer {
        struct Vertex *start;
        size_t count;
};

struct Face {
        int a[3];
        int b[3];
        int c[3];
};

struct FaceBuffer {
        struct Face *start;
        size_t count;
};

struct Buffer
buffer_alloc(size_t buffer_size);

void
buffer_free(struct Buffer buffer);

struct VertexBuffer
vertex_buffer_alloc(size_t count);

void
vertex_buffer_free(struct VertexBuffer vertex_buffer);

struct FaceBuffer
face_buffer_alloc(size_t count);

void
face_buffer_free(struct FaceBuffer face_buffer);

struct Buffer buffer_alloc(size_t buffer_size)
{
        struct Buffer buffer = {
                NULL,
                buffer_size
        };

        char *buffer_start = (char *)malloc(sizeof(char) * (buffer_size));
        if (buffer_start == NULL)
        {
                printf("%s\n", error_messages[ERROR_MALLOC_FAILED]);
        }
        else
        {
                buffer.start = buffer_start;
        }

        return buffer;
}

void buffer_free(struct Buffer buffer)
{
        free(buffer.start);
}

struct VertexBuffer vertex_buffer_alloc(size_t count)
{
        struct VertexBuffer vertex_buffer = {
                NULL,
                count
        };

        struct Vertex *vertex_buffer_start = (struct Vertex *)malloc(sizeof(struct Vertex)*count);
        if (vertex_buffer_start == NULL)
        {
                printf("%s\n", error_messages[ERROR_MALLOC_FAILED]);
        }
        else
        {
                vertex_buffer.start = vertex_buffer_start;
        }

        return vertex_buffer;
}

void vertex_buffer_free(struct VertexBuffer vertex_buffer) 
{
        free(vertex_buffer.start);
}

struct FaceBuffer face_buffer_alloc(size_t count)
{
        struct FaceBuffer face_buffer = {
                NULL,
                count
        };

        struct Face *face_buffer_start = (struct Face *)malloc(sizeof(struct Face)*count);
        if (face_buffer_start == NULL)
        {
                printf("%s\n", error_messages[ERROR_MALLOC_FAILED]);
        }
        else
        {
                face_buffer.start = face_buffer_start;
        }

        return face_buffer;
}

void face_buffer_free(struct FaceBuffer face_buffer)
{
        free(face_buffer.start);
}

#line 125 "buffers.cpp"
