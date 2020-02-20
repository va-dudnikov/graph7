/*!
 * \file
 */

#ifndef GRAPH7_ERROR_H
#define GRAPH7_ERROR_H

/*!
 * \name Error numbers
 * \{
 */

#define GRAPH7_INVALID_ARG      1   //!< Passed argument is wrong (e.g. NULL pointer)
#define GRAPH7_INVALID_LENGTH   2   //!< Passed argument of length is wrong
#define GRAPH7_INVALID_HEADER   3   //!< Invalid header
#define GRAPH7_INVALID_DATA     4   //!< Invalid data
#define GRAPH7_ALLOC_ERROR      5   //!< Allocator return NULL
#define GRAPH7_UNSUPPORTED      6   //!< If something unsupported

/*! \} */

#endif // GRAPH7_ERROR_H
