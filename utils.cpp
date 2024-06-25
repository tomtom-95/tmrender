#ifndef UTILS_CPP
#define UTILS_CPP

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

ErrorCode
print_error(const char *filename,
            int linenum,
            ErrorCode error_code);

ErrorCode
print_error(const char *filename,
            int linenum,
            ErrorCode error_code)
{
    printf("%s(%i), %s\n",
           filename,
           linenum,
           error_messages[error_code]);
    return error_code;
}

#endif // UTILS_CPP