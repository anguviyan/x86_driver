#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[])
{
	int fd,pos;
	char buf[20] = "sriramdhivakar";
	char rbuf[20];

	fd = open("/dev/sri_dev",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}


	write(fd,buf,sizeof(buf));
	pos = lseek(fd,0,SEEK_CUR);
	printf("The current seek position is %d\n",pos);
	
	getchar();

	pos = lseek(fd,0,SEEK_SET);
	printf("The current seek position is %d\n",pos);
	read(fd,rbuf,sizeof(rbuf));
	printf("Read consist of %s\n",rbuf);

	pos = lseek(fd,3,SEEK_SET);
	printf("The current seek position is %d\n",pos);
	read(fd,rbuf,sizeof(rbuf));
	printf("Read consist of %s\n",rbuf);
	pos = lseek(fd,3,SEEK_SET);
	printf("The current seek position is %d\n",pos);

	_exit(0);
}
