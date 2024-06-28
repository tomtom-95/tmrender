#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "tgaimage.cpp"
#include "utils.cpp"
#include "buffers.cpp"
// #include "parser.cpp"
// #include "wireframe_render.cpp"


int main(int argc, char** argv)
{
    struct Buffer filebuffer = ReadEntireFile(filename);
    BufferFree(&filebuffer);

    // size_t vertex_count = 0;
    // size_t face_count = 0;
    // size_t buffer_offset = 0;

    // while (buffer_offset != file_buffer.size)
    // {
    //     if (file_buffer.start[buffer_offset] == 'v' &&
    //         file_buffer.start[buffer_offset + 1] == ' ')
    //     {
    //         vertex_count++;
    //     }
    //     else if (file_buffer.start[buffer_offset] == 'f')
    //     {
    //         face_count++;
    //     }

    //     buffer_offset = read_line_from_buffer(file_buffer,
    //                                           buffer_offset);
    // }

    // struct VertexBuffer vertex_buffer = vertex_buffer_alloc(vertex_count);
    // if (vertex_buffer.start == NULL)
    // {
    //     printf("Cannot allocate vertex_buffer\n");
    //     return 1;
    // }

    // struct FaceBuffer face_buffer = face_buffer_alloc(face_count);
    // if (face_buffer.start == NULL)
    // {
    //     printf("Cannot allocate face_buffer\n");
    //     return 1;
    // }

    // parse_wavefront_obj(file_buffer,
    //                     vertex_buffer,
    //                     vertex_count,
    //                     face_buffer,
    //                     face_count);

    // // debug_vertex_buffer(vertex_buffer);

    // TGAImage image(SCREEN_WIDTH, SCREEN_HEIGHT, TGAImage::RGB);

    // struct Vertex vertices[3] = {
    //     vertex_buffer.start[17],
    //     vertex_buffer.start[18],
    //     vertex_buffer.start[19],
    // };

    // struct Triangle triangle;
    // triangle.vertices[0] = vertices[0];
    // triangle.vertices[1] = vertices[1];
    // triangle.vertices[2] = vertices[2];

    // // draw_triangle(triangle,
    // //               image);

    // render_wireframe(vertex_buffer,
    //                  face_buffer,
    //                  image,
    //                  WHITE);
    return 0;
}