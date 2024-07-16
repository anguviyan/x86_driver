#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
	int fd,rd;
	char buf[20];

	fd = open("/dev/sri_dev",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}

	rd = read(fd,buf,14);
	if(rd < 0)
	{
		perror("read");
		_exit(1);
	}

	printf("The read data is %s\n",buf);

	_exit(0);
}
