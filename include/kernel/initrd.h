#ifndef _INITRD_H_
#define _INITRD_H_

#include <kernel/fs.h>
#include <kernel/common.h>

struct initrd
{
        uint32_t nfiles;
}

struct initrd_file_header
{
        uint8_t magic;
        int8_t name[64];
        unsigned off_t offset;
        uint32_t length;
}

struct inode *initialize_initrd(uint32_t location);

#endif /* _INITRD_H_ */
