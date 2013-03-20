#include <kernel/fs.h>
#include <kernel/types.h>

struct fs_node *fs_root = NULL;

void open_fs(struct fs_node *node)
{
        if (node->fops.open != NULL)
                node->fops.open(node);
}

void close_fs(struct fs_node *node)
{
        if (node->fops.close != NULL)
                node->fops.close(node);
}

uint32_t read_fs(struct fs_node *node, off_t offset, size_t size, uint8_t *buffer)
{
        return node->fops.read != NULL?
                node->fops.read(node, offset, size, buffer): -1;
}

uint32_t write_fs(struct fs_node *node, off_t offset, size_t size, uint8_t *buffer)
{
        return node->fops.write != NULL?
                node->fops.write(node, offset, size, buffer): -1;
}

struct dirent *readdir_fs(struct fs_node *node, uint32_t index)
{
        if ((node->flags&0x7) == FS_DIRECTORY && node->fops.readdir != NULL)
                if (node->fops.readdir != 0)
                        return node->fops.readdir(node, index);
        return NULL;
}

struct fs_node *finddir_fs(struct fs_node *node, char *name)
{        
        if ((node->flags&0x7) == FS_DIRECTORY && node->fops.readdir != NULL)
                if (node->fops.finddir != 0)
                        return node->fops.finddir(node, name);
        return NULL;
}
