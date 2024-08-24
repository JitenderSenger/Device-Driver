#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
char buffer[2048];
int main(int argc,char *argv[])
{
	int fd,total_byte;
	int ret,remaining_data,retry=2;
	remaining_data=atoi(argv[1]);
 	fd = open("/dev/pcd_device_1",O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}	
	while(remaining_data && retry--)
	{
		ret = read(fd,buffer,remaining_data);
		if(ret == 0)
			break;
		total_byte+= ret;
		remaining_data -= ret;
	}
	printf("no of total bytes%d\n", total_byte);
	printf("Data :- %s\n",buffer);
	return 0;

}

