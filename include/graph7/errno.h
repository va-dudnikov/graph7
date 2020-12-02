#ifndef _GRAPH7_ERRNO_H
#define _GRAPH7_ERRNO_H

#define GRAPH7_INVALID_ARG      1   //!< Passed argument is wrong (e.g. NULL pointer)
#define GRAPH7_INVALID_LENGTH   2   //!< Passed argument of length is wrong
#define GRAPH7_INVALID_HEADER   3   //!< Invalid header
#define GRAPH7_INVALID_DATA     4   //!< Invalid data
#define GRAPH7_ALLOC_ERROR      5   //!< Allocator return NULL
#define GRAPH7_UNSUPPORTED      6   //!< If something unsupported

#define GRAPH7_ERROR(condition, error) do { if(condition) return error; } while(0)

static inline const char* graph7_strerror(int n)
{
    if(n < 0 || n > GRAPH7_UNSUPPORTED)
        return "Unknown error";

    static const char *messages[] =  {
        "No error",
        "Invalid argument",
        "Invalid length",
        "Invalid header",
        "Invalid data",
        "Allocation memory error",
        "Unsupported"
    };

    return messages[n];
}

#endif // _GRAPH7_ERRNO_H
