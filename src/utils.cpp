#ifndef UTILS_CPP
#define UTILS_CPP

#define K (1024)
#define M (1024 * 1024)
#define G (1024 * 1024 * 1024)

const TGAColor WHITE = TGAColor({255, 255, 255, 255});
const TGAColor BLACK = TGAColor({0,     0,   0,   0});

const TGAColor BLUE  = TGAColor({255,   0,   0,   0});
const TGAColor GREEN = TGAColor({0,   255,   0,   0});
const TGAColor RED   = TGAColor({0,     0, 255,   0});

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const char *filename = "./models/african_head/african_head.obj";

#define ERRORS \
    X(SUCCESS, success) \
    X(ERROR_MALLOC_FAILED, memory allocation failed) \
    X(ERROR_BUFFER_ALLOC_FAILED, buffer allocation failed) \
    X(ERROR_VERTEX_BUFFER_ALLOC_FAILED, vertex buffer allocation failed) \
    X(ERROR_FACE_BUFFER_ALLOC_FAILED, face buffer allocation failed) \
    X(ERROR_FILE_OPEN_FAILED, file opening failed) \
    X(ERROR_FILE_READ_FAILED, file reading failed)

typedef enum {
    #define X(error_code, error_message) error_code,
        ERRORS
    #undef X
} ErrorCode;

const char *error_messages[] = {
    #define X(error_code, error_message) #error_message,
        ERRORS
    #undef X
};

void
print_error(const char *filename,
            int linenum,
            const char *error_message);

void
swap_integers(int *x0, int *x1);

void
print_error(const char *filename,
            int linenum,
            const char *error_message)
{
    if (errno == 0)
    {
        printf("%s(%i), %s\n",
               filename,
               linenum,
               error_message);
    }
    else
    {
        printf("%s(%i), %s: %s\n",
            filename,
            linenum,
            error_message,
            strerror(errno));
    }
}

void
swap_integers(int *x0, int *x1)
{
    int tmp = *x0;
    *x0 = *x1;
    *x1 = tmp;
}

#endif // UTILS_CPP