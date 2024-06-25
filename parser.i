#line 1 "parser.cpp"



int parse_vertex(struct Buffer buffer,
                 size_t *buffer_offset_p,
                 struct VertexBuffer *vertex_buffer_p,
                 size_t *vertex_buffer_offset_p)
{
        char *line_start = buffer.start + *buffer_offset_p;
        size_t line_offset = 0;
        while (*(line_start + line_offset) != '\n')
        {
                line_offset++;
        }
        char *line = (char *)malloc(sizeof(char)*line_offset + 1);
        memcpy(line, line_start, line_offset);
        line[line_offset] = '\0';
        char tmp[2] = " ";
        sscanf(line, "%s %lf %lf %lf\r",
               &tmp,
               &(((vertex_buffer_p->start) + *vertex_buffer_offset_p) -> a),
               &(((vertex_buffer_p->start) + *vertex_buffer_offset_p) -> b),
               &(((vertex_buffer_p->start) + *vertex_buffer_offset_p) -> c));

        (*vertex_buffer_offset_p)++;
        *buffer_offset_p = *buffer_offset_p + line_offset + 1;
        printf("%c\n", *(buffer.start + *buffer_offset_p));
        return 0;
}

int res = parse_face(struct Buffer buffer,
                     size_t *buffer_offset_p,
                     struct FaceBuffer *face_buffer_p,
                     size_t *face_buffer_offset)
{
        return 0;
}

#line 40 "parser.cpp"
