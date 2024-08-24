#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11
struct pcddev_platform_data
{
	int size;
	int perm;
	char * serial_number;
};
