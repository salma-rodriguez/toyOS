#include <kernel/fs.h>


struct inode *fs_root = 0;

uint32_t read_fs(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->read(node, offset, size, buffer);
        else
                return 0;
}

uint32_t open(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->read(node, offset, size, buffer);
        else
                return 0;
}

uint32_t close(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->read(node, offset, size, buffer);
        else
                return 0;
}

uint32_t write(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->read(node, offset, size, buffer);
        else
                return 0;
}

uint32_t readdir(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{
        if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
                if (node->read != 0)
                        return node->read(node, offset, size, buffer);
                else
                        return 0;
}

uint32_t finddir(struct inode *node, off_t offset, size_t size, uint8_t *buffer)
{        
        if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
                if (node->read != 0)
                        return node->read(node, offset, size, buffer);
                else
                        return 0;
}
