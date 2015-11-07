#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      /* read(),write(),fork(),close(),...       */
#include <sys/socket.h> /* socket(), bind(), listen(), accept()... */
#include <netinet/in.h>  /* struct sockaddr_in,...                  */

#define PORT  (23456)

int main(int argc,char *argv[])
{
  int s1,s2;
  struct sockaddr_in    saddr,caddr;
  char buf[BUFSIZ];

  if( (s1=socket(AF_INET,SOCK_STREAM,0)) < 0 ) { 
    perror("socket");
    exit(1);
  }
  
  memset((char*)&saddr, 0, sizeof(saddr));
  saddr.sin_family      = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port        = htons(PORT);

  if( bind(s1, (struct sockaddr*)&saddr, sizeof(saddr))<0) {
    perror("bind");    exit(1);
  }

  if (listen(s1, 1) < 0) {
    perror("listen");    exit(1);
  }

  socklen_t len = sizeof(caddr);
  if ((s2 = accept(s1, (struct sockaddr *)&caddr, &len)) < 0) {
    perror("accept");    exit(1);
  }
  close(s1);

  strcpy(buf, "I'm a server.\n");
  write( s2, buf, sizeof(buf));
  
  close(s2);

  return 0;
}
