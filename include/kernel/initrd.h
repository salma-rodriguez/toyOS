#ifndef _INITRD_H_
#define _INITRD_H_

#include <kernel/fs.h>
#include <kernel/types.h>

struct initrd_header
{
        uint32_t nfiles;
};

struct initrd_file_header
{
        uint8_t magic;
        int8_t name[64];
        off_t offset;
        uint32_t length;
};

struct fs_node *initialize_initrd(uint32_t);

#endif /* _INITRD_H_ */
