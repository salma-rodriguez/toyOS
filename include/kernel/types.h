// simple type definitions
// next: 1. make POSIX-compliant by using posix_types.h
// 	 2. declare assembly types in another file (maybe)

#ifndef _TYPES_H_
#define _TYPES_H_

#define NULL ((void *) 0)

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
typedef		__s64	int64_t;
typedef 	__u8 	uint8_t;
typedef 	__u16 	uint16_t;
typedef 	__u32 	uint32_t;
typedef		__u64	uint64_t;

typedef int off_t;
typedef int ssize_t;
typedef unsigned int size_t;

#endif /* _TYPES_H_ */
