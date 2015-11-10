#include "common.h"
#include <time.h>
#include <sys/time.h>

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

  //********for client variation********
  int    s;
  char game_buf[BUFSIZ];
  //***********************************
  s = client_socket_procedure(server,server_port);
  
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
	  while(j < 5 && accept_list[j] != NULL) j++;
	  if(j != 5){
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

    read(s,game_buf,BUFSIZ);
    printf("%s\n",game_buf);
    
    if(accept_list[0] > 0){
      //      printf("%d", accept_list[0]);
      for(i=0 ; i < sizeof(accept_list)/sizeof(int) ; i++){
      	if(accept_list[i] > 0){
      	  write(accept_list[i], game_buf, sizeof(game_buf));
      	}
      }
    }
    if(strcmp("FORK", game_buf) == 0){
      printf("%s\n", game_buf);
      /* if(accept_list[1] > 0){ */
      /* 	write(accept_list[i], game_buf, sizeof(game_buf)); */
      /* } */
      if( (pid=fork()) <0 ) {
	perror("fork");
	return 1;
      } else if(pid==0) {
	copy_server(12345, server, 12346);
      }
    } //strcpy
  } //while 
  exit(0);
  close(s1);    
  //  close(s2_a);
  return 0;
  
  /* if (FD_ISSET(s1_a, &fds)) { */
  /*   sprintf(str1,"%d",count); */
  /*   strcpy(buf, str1); */
  /*   printf("count: %s\n", buf); */
  /*   write(s1_a, buf, sizeof(buf)); */
  /*   printf("output is s: %s\n", buf); */
  /* } */ 
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
