#ifndef _MCPP_NOLIBC_WCHAR_H
#define _MCPP_NOLIBC_WCHAR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* The conversion state the standard library asks for. Its contents are
 * unspecified; a C library's is larger only because it stores a partial
 * multibyte sequence, and nothing in the freestanding subset performs one. */
typedef struct { unsigned __count; unsigned __value; } mbstate_t;
typedef int wint_t;
#ifdef __cplusplus
}
#endif
#endif
