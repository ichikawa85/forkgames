#include "common.h"

static int copy_client(char* server , int port)
{
  int    s;
  char   buf[BUFSIZ];
  int    rc;

  s = client_socket_procedure(server,port);
  
  while( (rc=read(s,buf,BUFSIZ)) >0 ) {
    printf("%s\n",buf);
   }
  close(s);

  int s1, s2;
  fd_set fds, readfds;
  int maxfd;

  s1 = client_socket_procedure(server,);
  s2 = client_socket_procedure(server,port+1);

  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);
  FD_SET(s2, &readfds);

  if (s1 > s2) {
    maxfd = s1;
  } else {
    maxfd = s2;
  }

  while (1) {
    memcpy(&fds, &readfds, sizeof(fd_set));

    select(maxfd+1, &fds, NULL, NULL, NULL);

    if (FD_ISSET(s1, &fds)) {
      
      //      read(s1,buf,BUFSIZ);
      printf("output is s: %s\n", buf);
    }

    if (FD_ISSET(s2, &fds)) {
      //read(s2,buf,BUFSIZ);
      printf("output is s2: %s\n", buf);
    }
  }
  close(s1);
  close(s2);
  
  return 0;
}

int main(int argc,char *argv[])
{
  int port = PORT_NO;
  if( argc < 2 ) {
    printf("Usage: %s SERVER FILENAME [PORT]\n",argv[0]);
    return 1;
  }
  if( argc < 3) {
    port = PORT_NO;
  } else {
    port = atoi(argv[2]);
  }
  printf("port no. = %d\n", port);
  return   copy_client(argv[1],port);
}
