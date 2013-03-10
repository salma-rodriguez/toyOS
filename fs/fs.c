#include <kernel/fs.h>
#include <kernel/types.h>

struct fs_node *fs_root = NULL;

uint32_t read_fs(struct fs_node *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->read(node, offset, size, buffer);
        return 0;
}

void open_fs(struct fs_node *node)
{
        if (node->read != 0)
                node->open(node);
}

void close_fs(struct fs_node *node)
{
        if (node->read != 0)
                node->close(node);
}

uint32_t write_fs(struct fs_node *node, off_t offset, size_t size, uint8_t *buffer)
{
        if (node->read != 0)
                return node->write(node, offset, size, buffer);
        return 0;
}

struct dirent *readdir_fs(struct fs_node *node, uint32_t index)
{
        if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
                if (node->read != 0)
                        return node->readdir(node, index);
        return NULL;
}

struct fs_node *finddir_fs(struct fs_node *node, char *name)
{        
        if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
                if (node->read != 0)
                        return node->finddir(node, name);
        return NULL;
}
