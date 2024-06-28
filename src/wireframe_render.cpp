#ifndef WIREFRAME_RENDER_CPP
#define WIREFRAME_RENDER_CPP

int
check_out_of_screen_coords(int x0, int y0, int x1, int y1);

int
draw_line(int x0, int y0,
          int x1, int y1,
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

void
render_wireframe(struct VertexBuffer vertex_buffer,
                 struct FaceBuffer face_buffer,
                 TGAImage image,
                 TGAColor color);


int
check_out_of_screen_coords(int x0, int y0, int x1, int y1)
{
    bool out_of_coords = (x0 < 0 || x0 > SCREEN_WIDTH  ||
                          y0 < 0 || y0 > SCREEN_HEIGHT ||
                          x1 < 0 || x1 > SCREEN_WIDTH  ||
                          y0 < 0 || y0 > SCREEN_HEIGHT);
    if (out_of_coords == true)
    {
        printf("one of the coordinates is out of bounds:\n"
               "SCREEN_WIDTH=%i\n"
               "SCREEN_HEIGHT=%i\n"
               "x0=%i x1=%i y0=%i y1=%i\n\n",
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               x0, x1, y0, y1);
        return 1;
    }
    else
    {
        return 0;
    }
}

// TODO(tommaso): how to really implement bresenham algorithm?
int
draw_line(int x0, int y0,
          int x1, int y1,
          TGAImage &image,
          TGAColor color)
{
    int err = check_out_of_screen_coords(x0, y0, x1, y1);
    if (err == 1)
    {
        return 1;
    }
    
    int dx = x1 - x0;
    int dy = y1 - y0;

    int x_length = abs(dx);
    int y_length = abs(dy);
    float step = 0.0;
    if (x_length > y_length)
    {
        step = 1.0 / float(x_length);
    }
    else
    {
        step = 1.0 / float(y_length);
    }

    for (float t = 0; t < 1.0; t+=step)
    {
        image.set(x0 + dx * t, y0 + dy * t, color);
    }

    return 0;
}

// TODO(tommaso)
// compute triangle rasterization algorithm

// order vertex in ascending y coordinate order
struct Triangle
order_vertices(struct Triangle triangle)
{
    struct Vertex v0 = triangle.vertices[0];
    struct Vertex v1 = triangle.vertices[1];
    struct Vertex v2 = triangle.vertices[2];

    if (v0.components[1] > v1.components[1])
    {
        swap_vertices(&v0, &v1);
    }
    if (v0.components[1] > v2.components[1])
    {
        swap_vertices(&v0, &v2);
    }
    if (v1.components[1] > v2.components[1])
    {
        swap_vertices(&v1, &v2);
    }

    struct Triangle ordered_triangle = {
        v0, v1, v2
    };

    return ordered_triangle;
}

// TODO(tommaso): bugged
// check if the vertices in the triangle are in clockwise or anticlockwise order
bool
is_clockwise_triangle(struct Triangle triangle)
{
    struct Vertex v0 = triangle.vertices[0];
    struct Vertex v1 = triangle.vertices[1];
    struct Vertex v2 = triangle.vertices[2];

    double cross = (v1.components[0] * v2.components[1]) - (v1.components[1] * v2.components[0]);
    return cross > 0; 
}

double
edge_function(struct Vertex v0,
              struct Vertex v1,
              struct Vertex p)
{
    int first_term = (v1.components[0] - v0.components[0]) * (p.components[1] - v0.components[1]);
    int second_term = (v1.components[1] - v0.components[1]) * (p.components[0] - v0.components[0]);
    return first_term - second_term;
}

int
draw_triangle(struct Triangle triangle,
              TGAImage &image)
{
    triangle = order_vertices(triangle);

    struct Vertex v0 = triangle.vertices[0];
    struct Vertex v1 = triangle.vertices[1];
    struct Vertex v2 = triangle.vertices[2];

    // TODO: change that
    if (false) // if (is_clockwise_triangle(triangle))
    {
        // look for p such that the three edge_function are all positive
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            for (int j = 0; j < SCREEN_HEIGHT; j++)
            {
                struct Vertex p;
                p.components[0] = i;
                p.components[1] = j;
                p.components[0] = 0;

                if (edge_function(v0, v1, p) > 0 &&
                    edge_function(v1, v2, p) > 0 &&
                    edge_function(v2, v0, p) > 0)
                {
                    printf("HERE");
                    image.set(i, j, RED);
                }
            }
        }
    }
    else
    {
        // look for p such that the three edge_function are all negative
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            for (int j = 0; j < SCREEN_HEIGHT; j++)
            {
                struct Vertex p;
                p.components[0] = i;
                p.components[1] = j;
                p.components[0] = 0;

                if (edge_function(v0, v1, p) < 0 &&
                    edge_function(v1, v2, p) < 0 &&
                    edge_function(v2, v0, p) < 0)
                {
                    image.set(i, j, RED);
                }
            }
        }
    }

    image.write_tga_file("output.tga");

    return 0;
}


void
render_wireframe(struct VertexBuffer vertex_buffer,
                 struct FaceBuffer face_buffer,
                 TGAImage image,
                 TGAColor color)
{
    for (size_t face_idx = 0; face_idx < face_buffer.count; face_idx++)
    {
        int f0 = ((face_buffer.start + face_idx) -> components)[0];
        int f1 = ((face_buffer.start + face_idx) -> components)[3];
        int f2 = ((face_buffer.start + face_idx) -> components)[6];

        struct Vertex v0 = vertex_buffer.start[f0 - 1];
        struct Vertex v1 = vertex_buffer.start[f1 - 1];
        struct Vertex v2 = vertex_buffer.start[f2 - 1];

        long double x0 = (v0.components[0] + 1.0) * (SCREEN_WIDTH) / 2.0;
        long double y0 = (v0.components[1] + 1.0) * (SCREEN_HEIGHT) / 2.0;

        long double x1 = (v1.components[0] + 1.0) * (SCREEN_WIDTH) / 2.0;
        long double y1 = (v1.components[1] + 1.0) * (SCREEN_HEIGHT) / 2.0;

        long double x2 = (v2.components[0] + 1.0) * (SCREEN_WIDTH) / 2.0;
        long double y2 = (v2.components[1] + 1.0) * (SCREEN_HEIGHT) / 2.0;

        draw_line(x0, y0, x1, y1, image, color);
        draw_line(x0, y0, x2, y2, image, color);
        draw_line(x1, y1, x2, y2, image, color);
    }

    image.write_tga_file("output.tga");
}

#endif // WIREFRAME_RENDER_CPP