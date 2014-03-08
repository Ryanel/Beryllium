#include <stdio.h>
#include <vfs.h>
vfs_node_t *vsmounted_device_create();
vfs_node_t *null_device_create();
vfs_node_t *zero_device_create();
void init_devices()
{
	vfs_mount("/dev/null", null_device_create());
	vfs_mount("/dev/zero", zero_device_create());
	vfs_mount("/boot/vsmounted", vsmounted_device_create());
}