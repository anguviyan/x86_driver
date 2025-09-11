#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd,rd;

	char buf[6];

	fd = open("/dev/My_Drv_dev",O_RDWR);
	if(fd<0)
	{
		perror("open");
		_exit(1);
	}

	rd = read(fd,buf,sizeof(buf));
	if(rd < 0)
	{
		perror("read");
		_exit(1);
	}

	printf("Read consist of %d\n",rd);
	printf("Read data is %s\n",buf);

	close(fd);
	_exit(0);
}
