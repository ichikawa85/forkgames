#include "common.h"

int server_socket_procedure(int port)
{
  int s;
  struct sockaddr_in addr;

  s = socket(AF_INET,SOCK_STREAM,0);
  if( s  < 0 ) { 
    perror("socket");
    exit(1);
  }

  memset((char*)&addr, 0, sizeof(addr));

  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port        = htons(port);

  if( bind(s, (struct sockaddr*)&addr, sizeof(addr))<0) {
    perror("bind");
    exit(1);
  }

  if( listen(s,5)<0) {
    perror("listen");
    exit(1);
  }

  return s;
}

int client_socket_procedure(char* server,int port)
{
  int s;
  struct sockaddr_in  addr;
  struct hostent*     hp;

  if( (s=socket(AF_INET, SOCK_STREAM, 0)) <0 ) {
    perror("socket");
    exit(1);
  }

  memset((char*)&addr, 0, sizeof(addr));

  if( (hp=gethostbyname(server))==NULL) {
    perror("gethostbyname");
    exit(1);
  }

  bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);

  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0){
    perror("connect");
    exit(1);
  }

  return s;
}
