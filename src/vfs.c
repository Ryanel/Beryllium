#include <vfs.h>
#include <string.h>
vfs_node_t *vfs_root = 0;
vfs_node_t* vfs_getRootNode()
{
	return vfs_root;
}

void vfs_init()
{
	strcpy(vfs_root->name, "/");
	vfs_root->type = VFS_DIRECTORY | VFS_MOUNTPOINT;
	vfs_root->permissions = VFS_PERMISSION_READ | VFS_PERMISSION_WRITE ;
	vfs_root->uid = 0;
	vfs_root->gid = 0;
	vfs_root->inode = 0;
	vfs_root->length = 0;
	vfs_root->device = VFS_DEVICE_VFS; ///Internal device number
	vfs_printnode(vfs_root);

}
#ifdef DEBUG
#include <stdio.h>
void vfs_printnode(vfs_node_t *node)
{
	printf("Reading vfs node '%s'\n",node->name);
	printf("Type: ");
	if(node->type & VFS_FILE)
	{
		printf("file ");
	}
	if(node->type & VFS_DIRECTORY)
	{
		printf("directory ");
	}
	if(node->type & VFS_CHARDEVICE)
	{
		printf("chardev ");
	}
	if(node->type & VFS_BLOCKDEVICE)
	{
		printf("blockdev ");
	}
	if(node->type & VFS_PIPE)
	{
		printf("pipe ");
	}
	if(node->type & VFS_SYMLINK)
	{
		printf("symlink ");
	}
	if(node->type & VFS_MOUNTPOINT)
	{
		printf("mountpoint");
	}
	printf("\n");
	printf("perm: ");
	if(node->permissions & VFS_PERMISSION_READ)
	{
		printf("r");
	}
	if(node->permissions & VFS_PERMISSION_WRITE)
	{
		printf("w");
	}
	if(node->permissions & VFS_PERMISSION_EXECUTE)
	{
		printf("x");
	}
	printf("\n");
	printf("uid:%d, gid:%d\n",node->uid,node->gid);
	printf("inode:%d\n",node->inode);
	printf("size:%d bytes\n",node->length);
	printf("From device:");
	if(node->device & VFS_DEVICE_VFS)
	{
		printf("vfs");
	}
	else
	{
		printf("unkn");
	}
	printf("\n");
	if(node->type & VFS_SYMLINK)
	{
		printf("Symlinked / Mounted file stats:\n");
		vfs_printnode(node->ptr);
	}
}
#endif