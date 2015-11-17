#include    <stdio.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <sys/errno.h>
#include    <sys/fcntl.h>


void main()
{
  int   fd, fd2;
char  buf[256];

    if(mkfifo("/tmp/FifoTest",0666)==-1){
        perror("mkfifo");
    }

    if(mkfifo("/tmp/FifoTest2",0666)==-1){
      perror("mkfifo");
    }
    
    if((fd=open("/tmp/FifoTest",O_WRONLY))==-1){
        perror("open");
        exit(-1);
    }

    if((fd2=open("/tmp/FifoTest2",O_WRONLY))==-1){
      perror("open");
      exit(-1);
    }

    while(1){
        fgets(buf,sizeof(buf)-1,stdin);
        if(feof(stdin)){
            break;
        }
        write(fd,buf,strlen(buf));
	write(fd2,buf,strlen(buf));
    }
    close(fd);
    close(fd2);
}
