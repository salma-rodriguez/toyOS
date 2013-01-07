// simple type definitions
// next: 1. make POSIX-compliant by using posix_types.h
// 	 2. declare assembly types in another file (maybe)

#ifndef TYPES_H_
#define TYPES_H_

typedef char 		__s8;
typedef short 		__s16;
typedef int 		__s32;
typedef long		__s64;

typedef unsigned char 	__u8;
typedef unsigned short 	__u16;
typedef unsigned int 	__u32;
typedef unsigned long	__u64;

typedef 	__s8 	int8_t;
typedef 	__s16 	int16_t;
typedef 	__s32 	int32_t;
typedef 	__u8 	uint8_t;
typedef 	__u16 	uint16_t;
typedef 	__u32 	uint32_t;

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

#endif
