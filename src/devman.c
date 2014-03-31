#include <log.h>
#include <lib/tree.h>
#include <device.h>
#include <stdlib.h>
#include <string.h>
tree_t   * device_tree = NULL;
device_t * dev_root    = NULL;

void device_print_tree(tree_node_t * node, size_t height)
{
	/* End recursion on a blank entry */
	if (!node) return;
	/* Indent output */
	for (uint32_t i = 0; i < height; ++i) { printf("-"); }
	/* Get the current process */
	device_t * dnode = (device_t *)node->value;
	/* Print the process name */
	if (dnode->name) {
		printf(">%s",dnode->name);
	}
	/* Linefeed */
	printf("\n");
	foreach(child, node->children) {
		/* Recursively print the children */
		device_print_tree(child->value, height + 3);
	}
}

void deviceman_insert(device_t * device)
{
	//Note, change device_tree->root to parent from device_t
	tree_node_insert_child(device_tree, device_tree->root, device);
}

int deviceman_start()
{
	device_tree = tree_create();

	dev_root              = malloc(sizeof(device_t));
	dev_root->name        = "[device_root]";
	dev_root->type        = 0;
	dev_root->flags       = 0;
	dev_root->permissions = 0;
	dev_root->interface   = DEVICE_INTERFACE_KERNEL;
	tree_set_root(device_tree, dev_root);
	deviceman_insert(dev_root);
	device_print_tree(device_tree->root, 0);
	return 0;
}
