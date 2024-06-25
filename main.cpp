#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tgaimage.cpp"
#include "utils.cpp"
#include "buffers.cpp"
#include "parser.cpp"

const TGAColor white = TGAColor({255, 255, 255, 255});
const TGAColor red   = TGAColor({255, 0,   0,   255});

const int screen_width = 200;
const int screen_height = 200;

void swap_integers(int *x0, int *x1)
{
        int tmp = *x0;
        *x0 = *x1;
        *x1 = tmp;
        return;
}

int check_out_of_screen_coords(int x0, int y0, int x1, int y1)
{
        if (x0 < 0 || x0 > screen_width)
        {
                printf("x0 = %i is out of screen bound (screen is %i wide)\n", x0, screen_width);
                return 1;
        }
        if (y0 < 0 || y0 > screen_height)
        {
                printf("y0 = %i is out of screen bound (screen is %i high)\n", y0, screen_height);
                return 1;
        }
        if (x1 < 0 || x1 > screen_width)
        {
                printf("x1 = %i is out of screen bound (screen is %i wide)\n", x1, screen_width);
                return 1;
        }
        if (y0 < 0 || y0 > screen_height)
        {
                printf("x = %i is out of screen bound (screen is %i high)\n", y1, screen_height);
                return 1;
        }
        return 0;
}

int draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
        int err = check_out_of_screen_coords(x0, y0, x1, y1);
        if (err == 1)
        {
                printf("out of screen coords\n");
                return 1;
        }
        
        if (x0 > x1)
        {
                swap_integers(&x0, &x1);
        }
        if (y0 > y1)
        {
                swap_integers(&y0, &y1);
        }
        
        int x_length = x1 - x0;
        int y_length = y1 - y0;

        float step = 0.0;
        if (x_length > y_length)
        {
                step = 1.0 / float(x_length);
        }
        else
        {
                step = 1.0 / float(y_length);
        }
        for (float t = 0; t < 1; t+=step)
        {
                image.set(x0 + x_length * t, y0 + y_length * t, color);
        }

        return 0;
}

struct Buffer file_buffer_alloc(char *filename)
{
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL)
        {
                return print_error(__FILE__, __LINE__, ERROR_FILE_OPEN_FAILED);
        }
        fseek(fp, 0L, SEEK_END);
        long file_size = ftell(fp);
        rewind(fp);

        exit(0);

        size_t buffer_size = file_size + 1;
        struct Buffer buffer = buffer_alloc(buffer_size);
        if (buffer.start == NULL)
        {
                return print_error(__FILE__, __LINE__, ERROR_BUFFER_ALLOC_FAILED); 
        }

        // Read the file into the buffer
        size_t bytes_read = fread(buffer.start, sizeof(char), file_size, fp);
        if (bytes_read != file_size) {
                return print_error(__FILE__, __LINE__, ERROR_FILE_READ_FAILED);
        }

        buffer.start[buffer_size] = '\0';
}

ErrorCode
read_wavefront_obj_file(char *filename,
                        struct VertexBuffer *vertex_buffer_p,
                        struct FaceBuffer *face_buffer_p)
{
        size_t buffer_offset = 0;
        size_t vertex_count = 0;
        size_t face_count = 0;
        while (buffer.size != buffer_offset + 1)
        {
                if (buffer.start[buffer_offset] == 'v' &&
                    buffer.start[buffer_offset + 1] == ' ')
                {
                        vertex_count++;
                }
                else if (buffer.start[buffer_offset] == 'f')
                {
                        face_count++;
                }
                while (buffer.start[buffer_offset] != '\r' ||
                       buffer.start[buffer_offset + 1] != '\n')
                {
                        buffer_offset++;
                }
                buffer_offset += 2;
        }

        *vertex_buffer_p = vertex_buffer_alloc(vertex_count);
        if (vertex_buffer_p -> start == NULL)
        {
                return print_error(__FILE__, __LINE__, ERROR_VERTEX_BUFFER_ALLOC_FAILED);
        }

        *face_buffer_p = face_buffer_alloc(face_count);
        if (face_buffer_p -> start == NULL)
        {
                return print_error(__FILE__, __LINE__, ERROR_FACE_BUFFER_ALLOC_FAILED);
        }

        buffer_offset = 0;
        size_t face_buffer_offset = 0;
        size_t vertex_buffer_offset = 0;
        while (buffer.size != buffer_offset)
        {
                if (buffer.start[buffer_offset] == 'v' && 
                   (buffer.start[buffer_offset + 1]) == ' ')
                {
                        buffer_offset = parse_vertex(buffer,
                                                     buffer_offset,
                                                     *vertex_buffer_p,
                                                     vertex_buffer_offset);
                        vertex_buffer_offset++;
                }
                else if (buffer.start[buffer_offset] == 'f')
                {
                        int res = parse_face(buffer,
                                             &buffer_offset,
                                             *face_buffer_p,
                                             &face_buffer_offset);
                }
                else
                {
                        while (buffer.start[buffer_offset] != '\r' ||
                               buffer.start[buffer_offset + 1] != '\n')
                        {
                                buffer_offset++;
                        }
                        buffer_offset += 2;
                }
        }

        return SUCCESS;
}

int main(int argc, char** argv)
{
        TGAImage image(200, 200, TGAImage::RGB);
        draw_line(50, 200, 33, 68, image, white);
        // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");

        struct VertexBuffer vertex_buffer = {NULL, 0};
        struct FaceBuffer face_buffer = {NULL, 0};

        // TODO: write this functions
        // If I can assume that the vertex and faces are sequential, I can pass
        struct Buffer file_buffer = file_buffer_alloc();
        // size_t vertex_num = get_vertex_num();
        // size_t faces_num = get_faces_num();

        ErrorCode res = read_wavefront_obj_file("./obj/african_head/african_head_small.obj",
                                                &vertex_buffer,
                                                &face_buffer);
        
        debug_face_buffer(face_buffer);

        if (res != SUCCESS)
        {
                return res;
        }

        printf("PROGRAM FINISHED SUCCESSFULLY!");
        return 0;
}