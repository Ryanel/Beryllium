#include <vfs.h>
#include <string.h>
#include <log.h>
#include <lib/tree.h>
#include <stdlib.h>
#include <stdio.h>
tree_t     *vfs_tree = NULL;
void vfs_init()
{
	vfs_tree = tree_create();

	struct vfs_entry * root = malloc(sizeof(struct vfs_entry));
	root->name = strdup("[/]");
	root->file = NULL;
	tree_set_root(vfs_tree, root);

}
#ifdef DEBUG
#include <stdio.h>
void vfs_printnode(vfs_node_t *node)
{
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
}
void vfs_print_tree_node(tree_node_t * node, size_t height)
{
	/* End recursion on a blank entry */
	if (!node) return;
	/* Indent output */
	for (uint32_t i = 0; i < height; ++i) { printf(" "); }
	/* Get the current process */
	struct vfs_entry * fnode = (struct vfs_entry *)node->value;
	/* Print the process name */
	if (fnode->file) {
		printf("%s -> 0x%x (%s)", fnode->name, fnode->file, fnode->file->name);
	} else {
		printf("%s -> (empty)", fnode->name);
	}
	/* Linefeed */
	printf("\n");
	foreach(child, node->children) {
		/* Recursively print the children */
		vfs_print_tree_node(child->value, height + 1);
	}
}
#endif