#ifndef PARSER_CPP
#define PARSER_CPP

void
parse_vertex(struct Buffer buffer,
             size_t buffer_offset,
             struct VertexBuffer vertex_buffer,
             size_t vertex_buffer_offset);

void
debug_vertex_buffer(struct VertexBuffer vertex_buffer);

void
parse_face(struct Buffer buffer,
           size_t buffer_offset,
           struct FaceBuffer face_buffer,
           size_t face_buffer_offset);

void debug_face_buffer(struct FaceBuffer face_buffer);

void
parse_wavefront_obj(struct Buffer file_buffer,
                    struct VertexBuffer vertex_buffer,
                    size_t vertex_count,
                    struct FaceBuffer face_buffer,
                    size_t face_count);

void
parse_vertex(struct Buffer buffer,
             size_t buffer_offset,
             struct VertexBuffer vertex_buffer,
             size_t vertex_buffer_offset)
{
        // v -0.000581696 -0.734665 -0.623267\r\n
        buffer_offset += 2;
        for (int i = 0; i < 3; i++)
        {
                char *number_start_p = buffer.start + buffer_offset;
                char *number_end_p = NULL;
                long double num = strtold(number_start_p, &number_end_p);
                buffer_offset += (number_end_p - number_start_p) + 1;
                ((vertex_buffer.start + vertex_buffer_offset) -> components)[i] = num;
        }
}

void
debug_vertex_buffer(struct VertexBuffer vertex_buffer)
{
        for (int i = 0; i < vertex_buffer.count; i++)
        {
                printf("%i %Lf %Lf %Lf\n",
                        i + 1,
                       ((vertex_buffer.start + i) -> components)[0],
                       ((vertex_buffer.start + i) -> components)[1],
                       ((vertex_buffer.start + i) -> components)[2]);
        }
}

void
parse_face(struct Buffer buffer,
           size_t buffer_offset,
           struct FaceBuffer face_buffer,
           size_t face_buffer_offset)
{
        // f 24/1/24 25/2/25 26/3/26\r\n
        buffer_offset += 2;

        for (int i = 0; i < 9; i++)
        {
                size_t num_start_offset = buffer_offset;
                while (isdigit(buffer.start[buffer_offset]))
                {
                        buffer_offset++;
                }
                buffer_offset++;
                int num = atoi(buffer.start + num_start_offset);
                ((face_buffer.start + face_buffer_offset) -> components)[i] = num;
        }
}

void debug_face_buffer(struct FaceBuffer face_buffer)
{
        for (int i = 0; i < face_buffer.count; i++)
        {
                printf("%i %i/%i/%i %i/%i/%i %i/%i/%i\n",
                        i + 1,
                        ((face_buffer.start + i) -> components)[0],
                        ((face_buffer.start + i) -> components)[1],
                        ((face_buffer.start + i) -> components)[2],
                        ((face_buffer.start + i) -> components)[3],
                        ((face_buffer.start + i) -> components)[4],
                        ((face_buffer.start + i) -> components)[5],
                        ((face_buffer.start + i) -> components)[6],
                        ((face_buffer.start + i) -> components)[7],
                        ((face_buffer.start + i) -> components)[8]);
        }
}

void
parse_wavefront_obj(struct Buffer file_buffer,
                    struct VertexBuffer vertex_buffer,
                    size_t vertex_count,
                    struct FaceBuffer face_buffer,
                    size_t face_count)
{
        size_t buffer_offset = 0;
        for (size_t vertex_idx = 0; vertex_idx < vertex_count; vertex_idx++)
        {
                parse_vertex(file_buffer,
                             buffer_offset,
                             vertex_buffer,
                             vertex_idx);
                buffer_offset = read_line_from_buffer(file_buffer,
                                                      buffer_offset);
        }

        // debug_vertex_buffer(vertex_buffer);

        // TODO: temporary hack to go directly to faces part in the file
        while (file_buffer.start[buffer_offset] != 'f')
        {
                buffer_offset = read_line_from_buffer(file_buffer,
                                                      buffer_offset);
        }

        for (size_t face_idx = 0; face_idx < face_count; face_idx++)
        {
                parse_face(file_buffer,
                           buffer_offset,
                           face_buffer,
                           face_idx);
                buffer_offset = read_line_from_buffer(file_buffer,
                                                      buffer_offset);
        }
}

#endif // PARSER_CPP