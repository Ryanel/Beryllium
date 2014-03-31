#ifndef DRIVER_H
#define DRIVER_H
typedef struct {
	int owner; 
	int type;
	void* data;
} driver_msg_t;
#define DRIVER_STATUS_OFFLINE 0
#define DRIVER_STATUS_ONLINE  1
#define DRIVER_STATUS_ABORTED 2
typedef struct {
	char name[64];		          ///Name of driver
	int driver_id;		          ///Auto assigned
	unsigned char class;          ///Type of Driver
	unsigned char type;           ///Type of Driver
	int status;			///Is the driver loaded
	void* memory_region;///Pointer to the memory region owned by this driver
	int (*start)();
	int (*stop)();
	int (*recieve)(driver_msg_t*);
} driver_t;
#endif