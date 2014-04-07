#include <beryllium/vfs.h>
#include <log.h>
#include <stdlib.h>
#include <types.h>
#include <string.h>
vfs_node_t *initrd_root;
uint32_t read_initrd(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

uint32_t write_initrd(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

void open_initrd(vfs_node_t * node, unsigned int flags) {
	return;
}

void close_initrd(vfs_node_t * node) {
	return;
}
int vfs_initrd_init = 0;
vfs_node_t *initrd_device_create(uint32_t location, uint32_t end)
{
	if(vfs_initrd_init)
	{
		klog(LOG_WARN,"VFS","Attempted to remount the initrd to /boot!\n");
		return NULL;
	}
	initrd_root = malloc(sizeof(vfs_node_t));
	memset(initrd_root, 0x00, sizeof(vfs_node_t));
	initrd_root->inode = 0;
	strcpy(initrd_root->name, "initrd");
	initrd_root->uid = 0;
	initrd_root->gid = 0;
	initrd_root->permissions = VFS_PERMISSION_READ;
	initrd_root->flags   = VFS_DIRECTORY;
	initrd_root->read    = read_initrd;
	initrd_root->write   = write_initrd;
	initrd_root->open    = open_initrd;
	initrd_root->close   = close_initrd;
	initrd_root->readdir = NULL;
	initrd_root->finddir = NULL;
	return initrd_root;
}
