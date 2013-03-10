#include <kernel/types.h>

#define FS_FILE		0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03 
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define FS_SYMLINK	0x06
#define FS_MOUNTPOINT	0x08

struct fs_node;
extern struct fs_node *fs_root;

typedef uint32_t (*read_t)
(struct fs_node *, off_t, size_t, uint8_t *);

typedef uint32_t (*write_t)
(struct fs_node *, off_t, size_t, uint8_t *);

typedef void (*open_t)(struct fs_node *);

typedef void (*close_t)(struct fs_node *);

typedef struct dirent * (*readdir_t)
(struct fs_node *, uint32_t);

typedef struct fs_node * (*finddir_t)
(struct fs_node *, char *name); 

uint32_t read_fs(struct fs_node *, off_t, size_t, uint8_t *);
uint32_t write_fs(struct fs_node *, off_t, size_t, uint8_t *);
void open_fs(struct fs_node *);
void close_fs(struct fs_node *);
struct dirent *readdir_fs(struct fs_node *, uint32_t);
struct fs_node *finddir_fs(struct fs_node *, char *);

struct fs_node
{
	char name[128];
	uint32_t mask;
	uint32_t uid;
	uint32_t gid;
	uint32_t flags;
	uint32_t inode;
	uint32_t length;
	uint32_t impl;

	read_t read;
	write_t write;
	open_t open;
	close_t close;
	readdir_t readdir;
	finddir_t finddir;
	struct fs_node *ptr;
};



struct dirent
{
	char name[128];
	uint32_t ino;
};
