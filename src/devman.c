#include <log.h>
#include <lib/tree.h>
#include <device.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tree_t   * device_tree = NULL;

device_t * dev_root    = NULL;
device_t * kernel_ns   = NULL;

void device_tree_enumerate(tree_node_t * node, size_t height)
{

	if (!node) return;

	device_t * dnode = (device_t *)node->value;

	if (dnode) 
	{
		char *interface;
		char *type;
		switch(dnode->interface)
		{
			case DEVICE_INTERFACE_KERNEL:
				interface= "KERN";
				break;
			case DEVICE_INTERFACE_PCI:
				interface= "PCI";
				break;
			case DEVICE_INTERFACE_USB:
				interface= "USB";
				break;
			case DEVICE_INTERFACE_ACPI:
				interface= "ACPI";
				break;
			case DEVICE_INTERFACE_IO:
				interface= "IO";
				break;
			default:
				interface= "UNKN";
				break;
		}
		switch(dnode->type)
		{
			case DEVICE_TYPE_NAMESPACE:
				type= "NS";
				break;
			case DEVICE_TYPE_BUS:
				type= "BUS";
				break;
			case DEVICE_TYPE_HARDWARE:
				type= "HW";
				break;
			case DEVICE_TYPE_INTERGRATED:
				type= "VK";
				break;
			default:
				type= "??";
				break;
		}
		printf("%3s|",type);
		for (uint32_t i = 0; i < height; ++i) { printf(" "); }
		printf("%s (%s)",dnode->name,interface);
	}

	printf("\n");
	foreach(child, node->children) 
	{
		device_tree_enumerate(child->value, height + 4);
	}
}
uint8_t device_search_tree_comparator(void *a, void *b)
{
	if (a == b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
uint8_t device_search_name_comparator(void *a, void *b)
{
	device_t * dnode = (device_t *)a;
	if (strcmp(dnode->name,b) == 0)
	{
		return 1;
	}
	return 0;
}
tree_node_t * device_search_tree_node(device_t * device)
{
	tree_node_t * result;
	result = tree_find(device_tree, device, &device_search_tree_comparator);
	return result;
}

tree_node_t * device_search_node(char *name)
{
	tree_node_t * result;
	result = tree_find(device_tree, name, &device_search_name_comparator);
	return result;
}

device_t * device_search(char *name)
{
	tree_node_t * result;
	result = tree_find(device_tree, name, &device_search_name_comparator);
	return result->value;
}

void device_manager_insert(device_t * device,device_t * parent)
{
	//Note, change device_tree->root to parent from device_t
	tree_node_t * res = device_search_node(parent->name);
	if(res == NULL)
	{
		klog(LOG_WARN,"DRV","Inserting device %s failed. The parent does not exist in the device tree!\n");
		return;
	}
	tree_node_insert_child(device_tree, res, device);
}
void device_manager_remove(device_t * device)
{
	//Note, change device_tree->root to parent from device_t
	tree_node_remove(device_tree, device_search_tree_node(device));
}
int device_manager_start()
{
	device_tree = tree_create();

	dev_root              = malloc(sizeof(device_t));
	dev_root->name        = "device_root";
	dev_root->type        = 0;
	dev_root->flags       = 0;
	dev_root->permissions = 0;
	dev_root->interface   = DEVICE_INTERFACE_KERNEL;
	tree_set_root(device_tree, dev_root);
	device_manager_insert_kernel();
	device_tree_enumerate(device_tree->root, 0);
	return 0;
}

int has_inserted_staticially = 0;
extern device_t pit_device,serial_device,keyboard_device;
void device_manager_insert_kernel()
{
	if(has_inserted_staticially) return;
	klog(LOG_DEBUG,"DEV","Inserting kernel provided drivers...\n");
	///Setup the kernel namespace
	kernel_ns             = malloc(sizeof(device_t));
	kernel_ns->name       = "kernel_ns";
	kernel_ns->type       = DEVICE_TYPE_NAMESPACE;
	kernel_ns->flags      = 0;
	kernel_ns->interface  = DEVICE_INTERFACE_KERNEL;
	device_manager_insert(kernel_ns, device_search("device_root"));

	device_manager_insert(&pit_device, device_search("kernel_ns"));
	device_manager_insert(&serial_device, device_search("kernel_ns"));
	device_manager_insert(&keyboard_device, device_search("kernel_ns"));
	has_inserted_staticially = 1;
}