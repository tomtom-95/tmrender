#ifndef PARSER_CPP
#define PARSER_CPP

size_t parse_vertex(struct Buffer buffer,
                    size_t buffer_offset,
                    struct VertexBuffer vertex_buffer)
{
        // v -0.000581696 -0.734665 -0.623267\r\n
        
        buffer_offset += 2;

        while (buffer.start[buffer_offset] != '\r' ||
               buffer.start[buffer_offset + 1] != '\n')
        {
                for (int i = 0; i < 3; i++)
                {
                        char ch = buffer.start[buffer_offset];
                        if (ch == '+' || ch == '-' || isdigit(ch))
                        {
                                size_t number_start = buffer_offset;
                                char *number_end_p;
                                double num = strtod(buffer.start[number_start], &number_end_p);
                                buffer_offset += number_end_p - buffer.start[number_start];
                                ((vertex_buffer.start + vertex_buffer_offset) -> components)[i] = num;
                        }
                }
        }

        buffer_offset += 1;
        return buffer_offset
}

void debug_vertex_buffer(struct VertexBuffer vertex_buffer)
{
        for (int i = 0; i < vertex_buffer.count; i++)
        {
                printf("%i %lf %lf %lf\n",
                        i + 1,
                       ((vertex_buffer.start + i) -> components)[0],
                       ((vertex_buffer.start + i) -> components)[1],
                       ((vertex_buffer.start + i) -> components)[2]);
        }
}

int parse_face(struct Buffer buffer,
               size_t *buffer_offset_p,
               struct FaceBuffer face_buffer,
               size_t *face_buffer_offset_p)
{
        // f 24/1/24 25/2/25 26/3/26
        char *line_start_p = buffer.start + *buffer_offset_p;
        size_t line_offset = 0;
        int cnt = 0;
        while (*(line_start_p + line_offset) != '\n')
        {
                char ch = *(line_start_p + line_offset);
                if (ch == 'f' || ch == ' ') 
                {
                        line_offset++;
                }
                else if (isdigit(ch)) //  ->24/1/24
                {
                        for (int i = 0; i < 3; i++)
                        {
                                size_t num_start_offset = line_offset;
                                while (isdigit(*(line_start_p + line_offset)))
                                {
                                        line_offset++;
                                }
                                line_offset++;

                                int num = atoi(line_start_p + num_start_offset);
                                ((face_buffer.start + *face_buffer_offset_p)->components)[3*cnt+i] = num;
                        }
                        cnt++;
                }
        }
        *buffer_offset_p += (line_offset + 1);
        (*face_buffer_offset_p)++;

        return 0;
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

#endif // PARSER_CPP