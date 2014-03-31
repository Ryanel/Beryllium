#include <log.h>
#include <lib/tree.h>
tree_t * device_tree = NULL;
int deviceman_start()
{
	device_tree = tree_create();

	vfs_root = malloc(sizeof(struct vfs_entry));
	vfs_root->name = strdup("[root]");
	vfs_root->file =  NULL;
	tree_set_root(vfs_tree, vfs_root);
	return 0;
}