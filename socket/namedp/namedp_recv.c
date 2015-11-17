#include    <stdio.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <sys/errno.h>
#include    <sys/fcntl.h>


void main()
{
int   fd,len;
char  buf[256];
 int fd2, len2;

 if((fd=open("/tmp/FifoTest",O_RDONLY))==-1){
   perror("open");
   exit(-1);
 }

 /* if((fd2=open("/tmp/FifoTest2",O_RDONLY))==-1){ */
 /*   perror("open"); */
 /*   exit(-1); */
 /* } */

 while(1){
   len=read(fd,buf,sizeof(buf)-1);
   //   len2=read(fd2,buf,sizeof(buf)-1);
   
   if(len==0){
     break;
   }
   buf[len]='\0';
   fputs(buf,stdout);
 }
 close(fd);

 system("rm -f /tmp/FifoTest");
 // system("rm -f /tmp/FifoTest2");
}
