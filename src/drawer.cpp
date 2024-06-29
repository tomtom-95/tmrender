#ifndef WIREFRAME_RENDER_CPP
#define WIREFRAME_RENDER_CPP

int
ScreenCoordsCheck(int x0, int y0, int x1, int y1);

void
DrawLineLowSteep(int x_start, int y_start, int x_end, int y_end,
                 TGAImage &image,
                 TGAColor color);

void
DrawLineHighSteep(int x_start, int y_start, int x_end, int y_end,
                  TGAImage &image,
                  TGAColor color);

void
DrawLine(struct Vertex v0,
         struct Vertex v1,
         TGAImage &image,
         TGAColor color);

void
WireframeRender(struct VertexBuffer vertex_buffer,
                struct FaceBuffer face_buffer,
                TGAImage &image,
                TGAColor color);

struct Triangle
order_vertices(struct Triangle triangle);

bool
is_clockwise_triangle(struct Triangle triangle);

double
edge_function(struct Vertex v0,
              struct Vertex v1,
              struct Vertex p);

int
draw_triangle(struct Vertex vertices[3],
              TGAImage &image);


int
ScreenCoordsCheck(struct VertexBuffer vertex_buffer)
{
    for (size_t i = 0; i < vertex_buffer.count; i++)
    {
        if (vertex_buffer.data[i].x < 0 ||
            vertex_buffer.data[i].x > SCREEN_WIDTH ||
            vertex_buffer.data[i].y < 0 ||
            vertex_buffer.data[i].y > SCREEN_HEIGHT)
        {
            printf("coordinates out of bounds\n");
        }
        return 1;
    }

    return 0;
}

struct Vertex
VertexDenormalize(struct Vertex vertex)
{
    vertex.x = (vertex.x + 1.0) * (SCREEN_WIDTH / 2.0);
    vertex.y = (vertex.y + 1.0) * (SCREEN_HEIGHT / 2.0);

    return vertex;
}

void
DrawLineLowSteep(int x_start, int y_start, int x_end, int y_end,
                 TGAImage &image,
                 TGAColor color)
{
    int dx = x_end - x_start;
    int dy = y_end - y_start;

    double abs_m = abs(dy / (double)dx);
    double eps   = 1 - abs_m;

    int increment = 0;
    if (dy > 0)
    {
        increment = 1;
    }
    else
    {
        increment = -1;
    }

    int j = y_start;
    for (int i = x_start; i < x_end; i++)
    {
        if (eps > 1)
        {
            j += increment;
            eps -= 1.0;
        }

        eps += abs_m;
        image.set(i, j, color);
    }
}

void
DrawLineHighSteep(int x_start, int y_start, int x_end, int y_end,
                  TGAImage &image,
                  TGAColor color)
{
    int dx = x_end - x_start;
    int dy = y_end - y_start;

    double abs_m = abs(dx / (double)dy);
    double eps   = 1 - abs_m;

    int increment = 0;
    if (dx > 0)
    {
        increment = 1;
    }
    else
    {
        increment = -1;
    }

    int i = x_start;
    for (int j = y_start; j < y_end; j++)
    {
        if (eps > 1)
        {
            i += increment;
            eps -= 1.0;
        }

        eps += abs_m;
        image.set(i, j, color);
    }
}

// TODO(tommaso): how to really implement bresenham algorithm?
void
DrawLine(struct Vertex v0,
         struct Vertex v1,
         TGAImage &image,
         TGAColor color)
{
    int x_start = v0.x;
    int y_start = v0.y; 
    int x_end = v1.x;
    int y_end = v1.y;

    double dx = x_end - x_start;
    double dy = y_end - y_start;

    if (abs(dx) > abs(dy))
    {
        if (x_start < x_end)
        {
            DrawLineLowSteep(x_start, y_start, x_end, y_end, image, color);
        }
        else
        {
            DrawLineLowSteep(x_end, y_end, x_start, y_start, image, color);
        }
    }
    else
    {
        if (y_start < y_end)
        {
            DrawLineHighSteep(x_start, y_start, x_end, y_end, image, color);
        }
        else
        {
            DrawLineHighSteep(x_end, y_end, x_start, y_start, image, color);
        }
    }
}

void
WireframeRender(struct VertexBuffer vertex_buffer,
                struct FaceBuffer face_buffer,
                TGAImage &image,
                TGAColor color)
{
    for (size_t i = 0; i < face_buffer.count; i++)
    {
        int vertex_idx_0 = (((face_buffer.data + i) -> vertex_indices)[0]) - 1;
        int vertex_idx_1 = (((face_buffer.data + i) -> vertex_indices)[1]) - 1;
        int vertex_idx_2 = (((face_buffer.data + i) -> vertex_indices)[2]) - 1;

        if (i == 0)
        {
            printf("v0 = %i\n", vertex_idx_0);
            printf("v1 = %i\n", vertex_idx_1);
            printf("v2 = %i\n", vertex_idx_2);
        }
        
        struct Vertex v0 = VertexDenormalize(vertex_buffer.data[vertex_idx_0]);
        struct Vertex v1 = VertexDenormalize(vertex_buffer.data[vertex_idx_1]);
        struct Vertex v2 = VertexDenormalize(vertex_buffer.data[vertex_idx_2]);

        DrawLine(v0, v1, image, color);
        DrawLine(v0, v2, image, color);
        DrawLine(v1, v2, image, color);
    }
}

// TODO(tommaso)
// compute triangle rasterization algorithm

// order vertex in ascending y coordinate order
// struct Triangle
// order_vertices(struct Triangle triangle)
// {
//     struct Vertex v0 = triangle.vertices[0];
//     struct Vertex v1 = triangle.vertices[1];
//     struct Vertex v2 = triangle.vertices[2];
// 
//     if (v0.components[1] > v1.components[1])
//     {
//         swap_vertices(&v0, &v1);
//     }
//     if (v0.components[1] > v2.components[1])
//     {
//         swap_vertices(&v0, &v2);
//     }
//     if (v1.components[1] > v2.components[1])
//     {
//         swap_vertices(&v1, &v2);
//     }
// 
//     struct Triangle ordered_triangle = {
//         v0, v1, v2
//     };
// 
//     return ordered_triangle;
// }
// 
// // TODO(tommaso): bugged
// // check if the vertices in the triangle are in clockwise or anticlockwise order
// bool
// is_clockwise_triangle(struct Triangle triangle)
// {
//     struct Vertex v0 = triangle.vertices[0];
//     struct Vertex v1 = triangle.vertices[1];
//     struct Vertex v2 = triangle.vertices[2];
// 
//     double cross = (v1.components[0] * v2.components[1]) - (v1.components[1] * v2.components[0]);
//     return cross > 0; 
// }
// 
// double
// edge_function(struct Vertex v0,
//               struct Vertex v1,
//               struct Vertex p)
// {
//     int first_term = (v1.components[0] - v0.components[0]) * (p.components[1] - v0.components[1]);
//     int second_term = (v1.components[1] - v0.components[1]) * (p.components[0] - v0.components[0]);
//     return first_term - second_term;
// }
// 
// int
// draw_triangle(struct Triangle triangle,
//               TGAImage &image)
// {
//     triangle = order_vertices(triangle);
// 
//     struct Vertex v0 = triangle.vertices[0];
//     struct Vertex v1 = triangle.vertices[1];
//     struct Vertex v2 = triangle.vertices[2];
// 
//     // TODO: change that
//     if (false) // if (is_clockwise_triangle(triangle))
//     {
//         // look for p such that the three edge_function are all positive
//         for (int i = 0; i < SCREEN_WIDTH; i++)
//         {
//             for (int j = 0; j < SCREEN_HEIGHT; j++)
//             {
//                 struct Vertex p;
//                 p.components[0] = i;
//                 p.components[1] = j;
//                 p.components[0] = 0;
// 
//                 if (edge_function(v0, v1, p) > 0 &&
//                     edge_function(v1, v2, p) > 0 &&
//                     edge_function(v2, v0, p) > 0)
//                 {
//                     printf("HERE");
//                     image.set(i, j, RED);
//                 }
//             }
//         }
//     }
//     else
//     {
//         // look for p such that the three edge_function are all negative
//         for (int i = 0; i < SCREEN_WIDTH; i++)
//         {
//             for (int j = 0; j < SCREEN_HEIGHT; j++)
//             {
//                 struct Vertex p;
//                 p.components[0] = i;
//                 p.components[1] = j;
//                 p.components[0] = 0;
// 
//                 if (edge_function(v0, v1, p) < 0 &&
//                     edge_function(v1, v2, p) < 0 &&
//                     edge_function(v2, v0, p) < 0)
//                 {
//                     image.set(i, j, RED);
//                 }
//             }
//         }
//     }
// 
//     image.write_tga_file("output.tga");
// 
//     return 0;
// }


#endif // WIREFRAME_RENDER_CPP