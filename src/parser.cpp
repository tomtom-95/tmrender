#ifndef PARSER_CPP
#define PARSER_CPP

size_t
ReadBufferLine(struct Buffer buffer,
               size_t buffer_offset);

void
ParseVertex(struct Buffer buffer,
            size_t buffer_offset,
            struct VertexBuffer vertex_buffer,
            size_t vertex_buffer_offset);

void
PrintVertexBuffer(struct VertexBuffer vertex_buffer);

void
ParseFace(struct Buffer buffer,
          size_t buffer_offset,
          struct FaceBuffer face_buffer,
          size_t face_buffer_offset);

// TODO(tommaso): this is wrong! A file in windows could end a line with just '\n'
size_t
ReadBufferLine(struct Buffer buffer,
               size_t buffer_offset)
{
    for (;;)
    {
        if (buffer_offset + 1 == buffer.count)
        {
            return buffer_offset + 1;
        }
#if _WIN32
        else
        {
            if (buffer.data[buffer_offset] == '\r' &&
                buffer.data[buffer_offset + 1] == '\n')
            {
                return buffer_offset + 2;
            }
            else
            {
                buffer_offset++;
            }
        }
#else
        else
        {
            if (buffer.data[buffer_offset] == '\n')
            {
                return buffer_offset + 1;
            }
            else
            {
                buffer_offset++;
            }
        }
#endif
    }
}

void
ParseVertex(struct Buffer buffer, size_t buffer_offset,
            struct VertexBuffer vertex_buffer, size_t vertex_buffer_offset)
{
    // v -0.000581696 -0.734665 -0.623267\r\n
    buffer_offset += 2;

    double nums[3];
    for (int i = 0; i < 3; i++)
    {
        char *number_start = buffer.data + buffer_offset;
        char *number_end = NULL;
        nums[i] = strtold(number_start, &number_end);
        buffer_offset += (number_end - number_start) + 1;
    }

    (vertex_buffer.data + vertex_buffer_offset) -> x = nums[0];
    (vertex_buffer.data + vertex_buffer_offset) -> y = nums[1];
    (vertex_buffer.data + vertex_buffer_offset) -> z = nums[2];
}

void
PrintVertexBuffer(struct VertexBuffer vertex_buffer)
{
    for (size_t i = 0; i < vertex_buffer.count; i++)
    {
        printf("%i: %f %f %f\n",
               i + 1,
               vertex_buffer.data[i].x,
               vertex_buffer.data[i].y,
               vertex_buffer.data[i].z);
    }
}

void
ParseFace(struct Buffer buffer,
          size_t buffer_offset,
          struct FaceBuffer face_buffer,
          size_t face_buffer_offset)
{
    // f 24/1/24 25/2/25 26/3/26\r\n

    buffer_offset += 2;
    for (int i = 0; i < 3; i++)
    {
        int nums[3] = {};
        for (int j = 0; j < 3; j++)
        {
            size_t num_start_offset = buffer_offset;
            while (isdigit(buffer.data[buffer_offset]))
            {
                buffer_offset++;
            }
            buffer_offset++;
            nums[j] = atoi(buffer.data + num_start_offset);
        }
        ((face_buffer.data + face_buffer_offset) -> vertex_indices)[i]  = nums[0];
        ((face_buffer.data + face_buffer_offset) -> texture_indices)[i] = nums[1];
        ((face_buffer.data + face_buffer_offset) -> normal_indices)[i]  = nums[2];
    }
}

void
PrintFaceBuffer(struct FaceBuffer face_buffer)
{
    for (int i = 0; i < face_buffer.count; i++)
    {
        printf("%i: ", i + 1);
        for (int j = 0; j < 3; j++)
        {
            printf("%i/%i/%i ",
                   (face_buffer.data + i) -> vertex_indices[j],
                   (face_buffer.data + i) -> texture_indices[j],
                   (face_buffer.data + i) -> normal_indices[j]);
        }
        printf("\n");
    }
}

#endif // PARSER_CPP