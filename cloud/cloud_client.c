#include "common.h"

static int copy_client(char* server , int port)
{
  int    s;
  char   buf[BUFSIZ];
  char str1[128]={0};
  int key;
  int select_ret;
  struct timeval t_val = {0, 1000};
  fd_set fds, readfds;
  
  s = client_socket_procedure(server,port);
  //  write(s, "connect", sizeof(buf));
  
  /* while( (rc=read(s,buf,BUFSIZ)) >0 ) { */
  /*   printf("%s\n",buf); */
  /*  } */

  FD_ZERO(&readfds);
  FD_SET(s, &readfds);
  
  while(1){
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s+1, &fds, NULL, NULL, &t_val);

    if(select_ret != 0){
      if(FD_ISSET(s, &fds)){
	read(s,buf,BUFSIZ);
	printf("%s",buf);
      }
    }else{
      if(kbhit()){
	key = getch();
	if(key == 0 || key == 224) key = getch();
	sprintf(str1,"%d",key);
	strcpy(buf, str1);
	if(key == 113){ /* when press 'q' game end  */
	  close(s);  
	  return 0;
	}
	write(s, buf, sizeof(buf));
	
      }
    }
  }
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
