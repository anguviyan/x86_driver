#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int fd,wr;
	char wbuf[14] = "This is sriram";

	fd = open("/dev/sri_dev",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}

	wr = write(fd,wbuf,sizeof(wbuf));
	if(wr<0)
	{
		perror("write");
		_exit(1);
	}

	printf("write written these many data %d\n",wr);
	printf("Write written data is %s\n",wbuf);


	_exit(0);
}
