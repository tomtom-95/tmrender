#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "tgaimage.cpp"
#include "utils.cpp"
#include "buffers.cpp"
#include "parser.cpp"
#include "drawer.cpp"


int main(int argc, char** argv)
{
    struct Buffer file_buffer = ReadEntireFile(filename);

    // TODO(tommaso): why allocating G is a problem?
    // because I am compiling a 32 bit executable
    // TODO(tommaso): must compile a 64 bit executable
    // Maybe I have to use compiler at C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x86
    // TODO(tommaso): for now I allocate memory for 10M veritices and call it a day
    //                later on I will deal with actually caring more about memory
    struct VertexBuffer vertex_buffer = VertexBufferAllocate(10 * M);
    if (!vertex_buffer.data)
    {
        return 0;
    }
    struct FaceBuffer face_buffer = FaceBufferAlloc(10 * M);
    if (!face_buffer.data)
    {
        return 0;
    }
    // TODO(tommaso): the fact that vertex_buffer.count and face_buffer.count is not the actual number
    //                of vertex and faces is not cool!

    size_t file_buffer_offset   = 0;
    size_t vertex_buffer_offset = 0;
    size_t face_buffer_offset   = 0;
    size_t line_end_offset      = 0;
    while (file_buffer_offset < file_buffer.count - 1)
    {
        line_end_offset = ReadBufferLine(file_buffer,
                                         file_buffer_offset);
        if (*(file_buffer.data + file_buffer_offset) == 'v' &&
            *(file_buffer.data + file_buffer_offset + 1) == ' ')
        {
            ParseVertex(file_buffer, file_buffer_offset,
                        vertex_buffer, vertex_buffer_offset);
            vertex_buffer_offset++;
        }
        if (*(file_buffer.data + file_buffer_offset) == 'f')
        {
            ParseFace(file_buffer, file_buffer_offset,
                      face_buffer, face_buffer_offset);
            face_buffer_offset++;
        }

        file_buffer_offset = line_end_offset;
    }

    vertex_buffer.count = vertex_buffer_offset;
    face_buffer.count = face_buffer_offset;

    // PrintVertexBuffer(vertex_buffer);
    // PrintFaceBuffer(face_buffer);

    TGAImage image(SCREEN_WIDTH, SCREEN_HEIGHT, TGAImage::RGB);
    // WireframeRender(vertex_buffer,
    //                 face_buffer,
    //                 image,
    //                 WHITE);

    int light_direction[3] = {0, 0, -1};

    for (size_t i = 0; i < face_buffer.count; i++)
    {
        int vertex_idx_0 = (((face_buffer.data + i) -> vertex_indices)[0]) - 1;
        int vertex_idx_1 = (((face_buffer.data + i) -> vertex_indices)[1]) - 1;
        int vertex_idx_2 = (((face_buffer.data + i) -> vertex_indices)[2]) - 1;

        struct Vertex v0 = vertex_buffer.data[vertex_idx_0];
        struct Vertex v1 = vertex_buffer.data[vertex_idx_1];
        struct Vertex v2 = vertex_buffer.data[vertex_idx_2];
        
        struct Vector3D normal_vector = TriangleGetNormalVector(v0, v1, v2);
        normal_vector = VectorNormalize(normal_vector);

        double intensity = normal_vector.x * light_direction[0] +
                           normal_vector.y * light_direction[1] +
                           normal_vector.z * light_direction[2];

        v0 = VertexDenormalize(vertex_buffer.data[vertex_idx_0]);
        v1 = VertexDenormalize(vertex_buffer.data[vertex_idx_1]);
        v2 = VertexDenormalize(vertex_buffer.data[vertex_idx_2]);

        if (intensity > 0)
        {
            TGAColor color = TGAColor({(uint8_t)(intensity*255),
                                       (uint8_t)(intensity*255),
                                       (uint8_t)(intensity*255),
                                       255});
            ColorTriangle(v0, v1, v2,
                          image,
                          color);
        }

    }

    image.write_tga_file("output.tga");

    return 0;
}