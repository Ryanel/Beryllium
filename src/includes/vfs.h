#ifndef VFS_H
#define VFS_H
#include <types.h>
#include <lib/tree.h>
#define VFS_FILE        (1 << 0)
#define VFS_DIRECTORY   (1 << 1)
#define VFS_CHARDEVICE  (1 << 2)
#define VFS_BLOCKDEVICE (1 << 3)
#define VFS_PIPE        (1 << 4)
#define VFS_SYMLINK     (1 << 5)
#define VFS_MOUNTPOINT  (1 << 6)

#define VFS_PERMISSION_READ  0x01
#define VFS_PERMISSION_WRITE 0x02
#define VFS_PERMISSION_EXECUTE 0x04

#define VFS_DEVICE_VFS  (1 << 0)
#define VFS_DEVICE_RAMDISK (1 << 1)

typedef struct vfs_node{
	char name[256];
	uint32_t type;
	uint32_t flags;
	uint32_t permissions;
	uint32_t uid;
	uint32_t gid;
	uint32_t inode;
	uint32_t length;
	uint32_t device; ///Internal device number
	uint32_t device_type;
	struct vfs_node *ptr; // Used by mountpoints and symlinks.
} vfs_node_t;

struct vfs_entry {
	char * name;
	vfs_node_t * file;
};

vfs_node_t* vfs_getRootNode();
void vfs_printnode(vfs_node_t *node);
void vfs_init();
#ifdef DEBUG
void vfs_print_tree_node(tree_node_t * node, size_t height);
#endif
#endif