#include "common.h"
#include <time.h>
#include <sys/time.h>

int count = 0;

static int copy_server(int port)
{  
  int s1;
  int s1_a;
  struct sockaddr_in addr;
  struct timeval t_val = {0, 1000};
  int select_ret;
  socklen_t len = sizeof(addr);
  char buf[BUFSIZ];
  char str1[128]={0};  

  fd_set fds, readfds;
  int accept_list[5];
  int i, j;
  
  s1 = server_socket_procedure(port);

  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);

  while (1) {
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

    sprintf(str1,"%d",count);
    strcpy(buf, str1);
    printf("count: %s\n", buf);
    
    if(accept_list[0] > 0){
      //      printf("%d", accept_list[0]);
      for(i=0 ; i < sizeof(accept_list)/sizeof(int) ; i++){
      	if(accept_list[i] > 0){
      	  write(accept_list[i], buf, sizeof(buf));
      	}
      }
    }
    count++;
    sleep(3);	  
  }    
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
  int p_id;
  int port = PORT_NO;
  
  if( argc<2 ) {
    port = PORT_NO;
  } else {
    port = atoi(argv[1]);
  }
  printf("port no. = %d\n", port);

  return  copy_server(port);
}
