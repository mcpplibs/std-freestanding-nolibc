/* 最小 <string.h>:只声明 freestanding C++ 库真正会用到的东西 */
#ifndef _MCPP_NOLIBC_STRING_H
#define _MCPP_NOLIBC_STRING_H
#include <stddef.h>   /* 编译器自带,零 libc 档上可用 */
#ifdef __cplusplus
extern "C" {
#endif
void*  memcpy(void*, const void*, size_t);
void*  memmove(void*, const void*, size_t);
void*  memset(void*, int, size_t);
int    memcmp(const void*, const void*, size_t);
size_t strlen(const char*);
const void* memchr(const void*, int, size_t);
int    strcmp(const char*, const char*);
int    strncmp(const char*, const char*, size_t);
#ifdef __cplusplus
}
#endif
#endif
