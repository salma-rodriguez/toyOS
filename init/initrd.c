#include <string.h>
#include <kernel/heap.h>
#include <kernel/types.h>
#include <kernel/initrd.h>

struct initrd_header *initrd_header;
struct initrd_file_header *file_headers;

struct fs_node *initrd_dev;
struct fs_node *root_nodes;
struct fs_node *initrd_root;

int nroot_nodes;

struct dirent dirent;

static uint32_t initrd_read(struct fs_node *node, off_t offset, size_t size, uint8_t *buffer)
{
        struct initrd_file_header header;
        header = file_headers[node->inode];
        if (offset > header.length)
                return 0;
        if (offset+size > header.length)
                size = header.length-offset;
        memcpy(buffer, (uint8_t *)(header.offset+offset), size);
        return size;
}

static struct dirent *initrd_readdir(struct fs_node *node, uint32_t index)
{
        if (node == initrd_root && index == 0)
        {
                strcpy(dirent.name, "dev");
                dirent.name[3] = 0;
                dirent.ino = 0;
                return &dirent;
        }

        if (index-1 >= nroot_nodes)
                return 0;
        strcpy(dirent.name, root_nodes[index-1].name);
        dirent.name[strlen(root_nodes[index-1].name)] = 0;
        dirent.ino = root_nodes[index-1].inode;
        return &dirent;
}

static struct fs_node *initrd_finddir(struct fs_node *node, char *name)
{
        int i;
        if (node == initrd_root && !strcmp(name, "dev"))
                return initrd_dev;
        for (i = 0; i < nroot_nodes; i++)
                if (!strcmp(name, root_nodes[i].name))
                        return &root_nodes[i];
        return NULL;
}

struct fs_node *initialize_initrd(uint32_t location)
{
        int i;
        // root directory
        initrd_header = (struct initrd_header *)location;
        file_headers = (struct initrd_file_header *)(location+sizeof(struct initrd_header));
        initrd_root = (struct fs_node *)kmalloc(sizeof(struct fs_node));
        initrd_root->uid = 0;
        initrd_root->gid = 0;
        initrd_root->mask = 0;
        initrd_root->inode = 0;
        initrd_root->length = 0;
        initrd_root->flags = FS_DIRECTORY;
        initrd_root->read = NULL;
        initrd_root->write = NULL;
        initrd_root->open = NULL;
        initrd_root->close = NULL;
        initrd_root->readdir =  &initrd_readdir;
        initrd_root->finddir = &initrd_finddir;
        initrd_root->ptr = NULL;
        initrd_root->impl = 0;
        // /dev directory
        initrd_dev = (struct fs_node *)kmalloc(sizeof(struct fs_node));
        strcpy(initrd_dev->name, "dev");
        initrd_dev->mask = 0;
        initrd_dev->uid = 0;
        initrd_dev->gid = 0;
        initrd_dev->inode = 0;
        initrd_dev->length = 0;
        initrd_dev->flags = FS_DIRECTORY;
        initrd_dev->read = NULL;
        initrd_dev->write = NULL;
        initrd_dev->open = NULL;
        initrd_dev->close = NULL;
        initrd_dev->readdir = &initrd_readdir;
        initrd_dev->finddir = &initrd_finddir;
        initrd_dev->ptr = NULL;
        initrd_dev->impl = 0;

        root_nodes = (struct fs_node *)kmalloc(sizeof(struct fs_node) * initrd_header->nfiles);
        nroot_nodes = initrd_header->nfiles;

        for (i = 0; i < initrd_header->nfiles; i++)
        {
                file_headers[i].offset += location;
                strcpy(root_nodes[i].name, (char *)&file_headers[i].name);
                root_nodes[i].mask = root_nodes[i].uid = 0;
                root_nodes[i].gid = 0;
                root_nodes[i].length = file_headers[i].length;
                root_nodes[i].inode = i;
                root_nodes[i].flags = FS_FILE;
                root_nodes[i].read = &initrd_read;
                root_nodes[i].write = 0;
                root_nodes[i].readdir = 0;
                root_nodes[i].finddir = 0;
                root_nodes[i].open = 0;
                root_nodes[i].close = 0;
                root_nodes[i].impl = 0;
        }

        return initrd_root;
}
