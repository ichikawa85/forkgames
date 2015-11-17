#include "common.h"

static int copy_client(char* server , int port)
{
  int    s, s2;
  char   buf[BUFSIZ];
  int    rc;
  struct sockaddr_in addr;
  struct timeval t_val = {0, 1000};
  int select_ret;
  
  fd_set fds, readfds;
  int maxfd;
  
  s = client_socket_procedure(server,port);
  //  s2 = client_socket_procedure(server,port+1);

  FD_ZERO(&readfds);
  FD_SET(s, &readfds);
  //  FD_SET(s2, &readfds);

  maxfd = s;
  
  /* if (s > s2) { */
  /*   maxfd = s; */
  /* } else { */
  /*   maxfd = s2; */
  /* } */

  while(1){
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(0, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s, &fds)){
	memset(buf, 0, BUFSIZ);
	recv(s, buf, BUFSIZ, 0);
	buf[BUFSIZ-1] = '\0';
	printf("%s\n", buf);
      }
    }else{
      if(fgets(buf,BUFSIZ-1, stdin) != NULL){
	// データ送信
	printf("write\n");
	//sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
	write(s, buf, sizeof(buf));
      }
    }
  }

  /* while (1) { */
  /*   memcpy(&fds, &readfds, sizeof(fd_set)); */
  /*   select(maxfd+1, &fds, NULL, NULL, NULL); */
  /*   if (FD_ISSET(s, &fds)) { */
  /*     read(s,buf,BUFSIZ); */
  /*     printf("output is s: %s\n", buf); */
  /*   } */
  /*   if (FD_ISSET(s2, &fds)) { */
  /*     read(s2,buf,BUFSIZ); */
  /*     printf("output is s2: %s\n", buf); */
  /*   } */
  /* } */

  close(s);
  //  close(s2);
  
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
