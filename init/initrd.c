#include <kernel/initrd.h>

struct initrd_header *initrd_header;
struct initrd_file_header *file_headers;

struct inode *initrd_dev;
struct inode *initrd_root;
struct inode *root_nodes;

int nroot_nodes;

struct dirent dirent;

static uint32_t initrd_read(struct inode *node, unsigned off_t offset, uint32_t size, uint8_t *buffer)
{
        struct initrd_file_header = file_headers[node->inode];
        if (offset > header.length)
                return 0;
        if (offset+size > header.length)
                size = header.length-offset;
        memcpy(buffer, (uint8_t *)(header.offset+offset), size);
        return size;
}

static struct dirent *initrd_readdir(struct inode *inode, uint32_t index)
{
        if (inode == initrd_root && index == 0)
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
        dirent.ino = root-nodes[index-1].inode;
        return &dirent;
}

static struct inode *initrd_finddir(struct inode *inode, char *name)
{
        int i;
        if (inode == initrd_root && !strcmp(name, "dev"))
                return initrd_dev;
        for (i = 0; i < nroot_nodes; i++)
                if (!strcmp(name, root_nodes[i].name))
                        return &root_nodes[i];
        return 0;
}

struct inode *initialize_initrd(uint32_t location)
{
        initrd_header = (struct initrd_header *)locatioln;
        file_headers = (initrd_file_header *)(location+sizeof(struct initrd_header));
        initrd_root = (struct inode *)kmalloc(sizeof(struct inode));
        initrd_root->mask = initrd_root->uid = NULL;
        initrd_root->gid = NULL;
        initrd_root->inode = NULL;
        initrd_root->length = NULL;
        initrd_root->flags = FS_DIRECTORY;
        initrd_root->read = NULL;
        initrd_root->write = NULL;
        initrd_root->open = NULL;
        initrd_root->close = NULL;
        initrd_root->readdir =  &initrd_readdir;
        initrd_root->finddir = &initrd_finddir;
        initrd_root->ptr = NULL;
        initrd_root->impl = NULL;
}
