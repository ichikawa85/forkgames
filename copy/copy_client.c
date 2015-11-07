#include "common.h"

static int copy_client(char* server,  char* fn , int port)
{
  int    s;
  char   buf[BUFSIZ];
  int    rc;
  
  s = client_socket_procedure(server,port);
  strcpy(buf, fn);
  write(s ,buf , strlen(buf));

  while( (rc=read(s,buf,BUFSIZ)) >0 ) {
    printf("%s",buf);
  }

  close(s);
  
  return 0;
}

int main(int argc,char *argv[])
{
  int port = PORT_NO;
  if( argc < 3 ) {
    printf("Usage: %s SERVER FILENAME [PORT]\n",argv[0]);
    return 1;
  }
  if( argc < 4) {
    port = PORT_NO;
  } else {
    port = atoi(argv[3]);
  }
  printf("port no. = %d\n", port);
  return   copy_client(argv[1],argv[2],port);
}
