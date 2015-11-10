#include "common.h"
#include <time.h>
#include <sys/time.h>

#define FD_SETSIZE 5

static int copy_server(int server_port,char* server ,int client_port)
{  
  int s1;
  //  int s1_a;
  //  struct sockaddr_in addr;
  struct timeval t_val = {0, 1000};
  int select_ret;
  //socklen_t len = sizeof(addr);
  char buf[BUFSIZ];
  //  char str1[128]={0};  

  fd_set fds, readfds;
  int accept_list[5];
  int i, j;

  s1 = server_socket_procedure(client_port);

  //********for self client variation********
  int    s[FD_SETSIZE];
  char game_buf[BUFSIZ];
  int fork_num = 0;
  //***********************************
  s[fork_num++] = client_socket_procedure(server,server_port);
  
  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);

  int pid;

  while (1) {
    //*************  MANAGE ACCEPT LIST  *****************
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s1+1, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s1, &fds)){
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	int client_sock = accept(s1, (struct sockaddr *)&client, &len);
	if(client_sock != NULL){
	  j=0;
	  while(j < FD_SETSIZE && accept_list[j] != NULL) j++;
	  if(j != FD_SETSIZE){
	    FD_SET(client_sock, &readfds);
	    accept_list[j] = client_sock;
	    printf("accept\n");
	  }else{
	    printf("no empty\n");
	  }
	}else{
	  printf("accept error\n");
	}
      }
    }
    
    int k=0;
    //*************  Read from server input  **************
    for(k=0 ; k<fork_num ; k++){
      read(s[k],game_buf,BUFSIZ);
      printf("%s\n",game_buf);
    }

    //*************  Write game_buf for client  **************
    if(accept_list[0] > 0){
      for(i=0 ; i < sizeof(accept_list)/sizeof(int) ; i++){
      	if(accept_list[i] > 0){
      	  write(accept_list[i], game_buf, sizeof(game_buf));
      	}
      }
    }

    //**************  Connect new port server when forked it  ********
    if(strcmp("FORK", game_buf) == 0){
      sleep(1);
      s[fork_num++] = client_socket_procedure(server,12345);
      printf("fork number of server: %d¥n" fork_num);
    } //strcpy
  } //while 
  exit(0);
  close(s1);    
  //  close(s2_a);
  return 0;
}

int main(int argc,char *argv[])
{
  //  int port = PORT_NO;
  int server_port;
  int client_port;
  
  if( argc<2 ) {
    //    port = PORT_NO;
    return printf("Please input port Number/n");
  } else {
    server_port = atoi(argv[1]);
    client_port = atoi(argv[3]);
  }
  printf("for server port no. = %d\n", server_port);
  printf("for client port no. = %d\n", client_port);

  return  copy_server(server_port, argv[2],client_port);
}
