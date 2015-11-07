#include "common.h"

static int copy_client(char* server , int port)
{
  int    s;
  char   buf[BUFSIZ];
  int    rc;
  
  s = client_socket_procedure(server,port);
  //  write(s, "connect", sizeof(buf));
  
  while( (rc=read(s,buf,BUFSIZ)) >0 ) {
    printf("%s\n",buf);
   }

  /* while(1) { */
  /*   read(s,buf,BUFSIZ); */
  /*   printf("%s\n",buf); */
  /* } */

  close(s);
  
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
