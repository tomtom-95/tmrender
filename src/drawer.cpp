#ifndef DRAWER_CPP
#define DRAWER_CPP

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

int
GetEdgeFunction(struct Vertex A,
                struct Vertex B,
                struct Vertex C);

void
ColorTriangle(struct Vertex A,
              struct Vertex B,
              struct Vertex C,
              TGAImage &image,
              TGAColor color);

struct Vector3D
TriangleGetNormalVector(struct Vertex A,
                        struct Vertex B,
                        struct Vertex C);

struct Vector3D
NormalVectorNormalize(struct Vector3D normal_vector);


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
        
        struct Vertex v0 = VertexDenormalize(vertex_buffer.data[vertex_idx_0]);
        struct Vertex v1 = VertexDenormalize(vertex_buffer.data[vertex_idx_1]);
        struct Vertex v2 = VertexDenormalize(vertex_buffer.data[vertex_idx_2]);

        DrawLine(v0, v1, image, color);
        DrawLine(v0, v2, image, color);
        DrawLine(v1, v2, image, color);
    }
}

int
GetTriangleDeterminant(struct Vertex v0,
                       struct Vertex v1,
                       struct Vertex v2)
{
    int determinant = (v1.x * v2.y + v0.x * v1.y + v0.x * v2.x) -
                      (v0.y * v1.x + v1.y * v2.x + v0.x * v2.y);
    return determinant;
}

int
GetEdgeFunction(struct Vertex A,
                struct Vertex B,
                struct Vertex C)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

void
ColorTriangle(struct Vertex A,
              struct Vertex B,
              struct Vertex C,
              TGAImage &image,
              TGAColor color)
{
    // TODO(tommaso): must get a bounding box for the triangle
    //                now this is slow as fuck
    int edge = GetEdgeFunction(A, B, C);
    if (edge < 0)
    {
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
            for (int j = 0; j < SCREEN_WIDTH; j++)
            {
                struct Vertex P = {i, j, 0};
                if (GetEdgeFunction(A, B, P) < 0 &&
                    GetEdgeFunction(B, C, P) < 0 &&
                    GetEdgeFunction(C, A, P) < 0)
                {
                    image.set(P.x, P.y, color);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
            for (int j = 0; j < SCREEN_WIDTH; j++)
            {
                struct Vertex P = {i, j, 0};
                if (GetEdgeFunction(A, B, P) > 0 &&
                    GetEdgeFunction(B, C, P) > 0 &&
                    GetEdgeFunction(C, A, P) > 0)
                {
                    image.set(P.x, P.y, color);
                }
            }
        }
    }
}

struct Vector3D
TriangleGetNormalVector(struct Vertex A,
                        struct Vertex B,
                        struct Vertex C)
{
    struct Vertex v0 = {C.x - A.x, C.y - A.y, C.z - A.z};
    struct Vertex v1 = {B.x - A.x, B.y - A.y, B.z - A.z};

    struct Vector3D normal_vector;

    // compute the cross product
    normal_vector.x =   v0.y * v1.z - v0.z * v1.y;
    normal_vector.y = -(v0.x * v1.z - v0.z * v1.x);
    normal_vector.z =   v0.x * v1.y - v0.y * v1.x;

    return normal_vector;
}

struct Vector3D
VectorNormalize(struct Vector3D vector)
{
    double vector_modulus = sqrt(vector.x * vector.x + 
                                 vector.y * vector.y +
                                 vector.z * vector.z);
    vector.x = vector.x / vector_modulus;
    vector.y = vector.y / vector_modulus;
    vector.z = vector.z / vector_modulus;

    return vector;
}

#endif // DRAWER_CPP