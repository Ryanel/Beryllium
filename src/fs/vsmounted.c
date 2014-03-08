#include <vfs.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>
uint32_t read_vsmounted(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_vsmounted(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_vsmounted(vfs_node_t *node, unsigned int flags);
void close_vsmounted(vfs_node_t *node);

uint32_t read_vsmounted(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

uint32_t write_vsmounted(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

void open_vsmounted(vfs_node_t * node, unsigned int flags) {
	return;
}

void close_vsmounted(vfs_node_t * node) {
	return;
}

vfs_node_t *vsmounted_device_create()
{
	vfs_node_t * fnode = malloc(sizeof(vfs_node_t));
	memset(fnode, 0x00, sizeof(vfs_node_t));
	fnode->inode = 0;
	strcpy(fnode->name, "vsmounted");
	fnode->uid = 0;
	fnode->gid = 0;
	fnode->permissions = VFS_PERMISSION_READ;
	fnode->flags   = VFS_FILE | VFS_CHARDEVICE;
	fnode->read    = read_vsmounted;
	fnode->write   = write_vsmounted;
	fnode->open    = open_vsmounted;
	fnode->close   = close_vsmounted;
	fnode->readdir = NULL;
	fnode->finddir = NULL;
	return fnode;
}