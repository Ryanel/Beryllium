#include <vfs.h>
#include <string.h>
#include <log.h>
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
	vfs_root->ptr = vfs_root;
	vfs_printnode(vfs_root);

}
#ifdef DEBUG
#include <stdio.h>
int vfs_recurse = 0;
void vfs_printnode(vfs_node_t *node)
{
	if(vfs_recurse == 2)
	{
		printf("Exiting recursion...\n");
		return;
	}
	klog(LOG_DEBUG,"VFS","Reading vfs node '%s'\n",node->name);
	klog(LOG_DEBUG,"VFS","Type: ");
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
	klog(LOG_DEBUG,"VFS","perm: ");
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
	klog(LOG_DEBUG,"VFS","uid:%d, gid:%d\n",node->uid,node->gid);
	klog(LOG_DEBUG,"VFS","inode:%d\n",node->inode);
	klog(LOG_DEBUG,"VFS","size:%d bytes\n",node->length);
	klog(LOG_DEBUG,"VFS","From device:");
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
		klog(LOG_DEBUG,"VFS","Symlinked file stats:\n");
		vfs_recurse++;
		vfs_printnode(node->ptr);
	}
}
#endif