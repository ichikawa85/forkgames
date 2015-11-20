#include "common.h"
#include <time.h>
#include <sys/time.h>

//#define FD_SETSIZE 5
#define MAXITEM 2

int split(char *str, const char *delim, char *outlist[]) {
  char    *tk;
  int     cnt = 0;
  
  tk = strtok( str, delim );
  while( tk != NULL && cnt < MAXITEM ) {
    outlist[cnt++] = tk;
    tk = strtok( NULL, delim );
  }
  return cnt;
}

static int proxy_server(int server_port,char* server ,int client_port)
{
  int s1;
  struct timeval t_val = {0, 1000};
  int select_ret, select_ret_client;
  char buf[BUFSIZ];
  //  char str1[128]={0};  
  
  fd_set fds, readfds, fds_client, readfds_client;
  int accept_list[5];
  int i, j, c;
  
  s1 = server_socket_procedure(client_port);
  
  //********for self client variation********
  int s[FD_SETSIZE];
  char game_buf[BUFSIZ];
  int fork_num = 0;
  //***********************************
  struct sock_list {
    int client[5];
    int server;
  };
  struct sock_list table[5];
  
  s[fork_num++] = client_socket_procedure(server,server_port);
  table[0].server = s[0];
  
  FD_ZERO(&readfds);
  FD_SET(s1, &readfds);
  
  int pid;
  while (1) {
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s1+accept_list[0]+1, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s1, &fds)){ /* manage accept list */
    	struct sockaddr_in client;
    	socklen_t len = sizeof(client);
    	int client_sock = accept(s1, (struct sockaddr *)&client, &len);
    	if(client_sock != 0){
    	  j=0;
    	  while(j < FD_SETSIZE && accept_list[j] != 0) j++;
    	  if(j != FD_SETSIZE){
    	    FD_SET(client_sock, &readfds);
    	    accept_list[j] = client_sock;
	    table[0].client[j] = accept_list[j];
	    /* my addition */
	    
	    /* c=0; */
	    /* while(c < 5 && table[0].client[c] != 0) c++; */
	    /* if(c != 5){ */
	    /*   printf("table[%d].client=%d\n", j, table[j].client[c]); */
	    /*   table[0].client[c] = accept_list[j]; */
	    /* } */
	    
	    /********************/
    	    printf("accept\n");
    	  }else{
    	    printf("no empty\n");
    	  }
    	}else{
    	  printf("accept error\n");
    	}
      }
      /* Write for server from client  (client -> proxy -> server) */
      int w, v;
      for(w=0 ; w < fork_num ; w++){
	for(v=0 ; v < sizeof(table[w].client)/sizeof(int) ; v++){
	  if(FD_ISSET(table[w].client[v], &fds)){
	    read(table[w].client[v] ,buf,BUFSIZ);
	    write(table[w].server, buf, sizeof(buf));
	    printf("%s\n",buf);
	  }
	}
      }
    }      
    int m, k;
    /*  Read from server input  (server -> proxy -> client) */
    for(k=0 ; k<fork_num ; k++){
      char a_buf[BUFSIZ];
      char str1[128]={0};
      int cnt;
      read(table[k].server,game_buf,BUFSIZ);
      //printf("table[%d].server=%d\n", k, table[k].server);
      for(m = 0 ; m < sizeof(table[k].client)/sizeof(int) ; m++){
	//printf("table[%d].cleint=%d \n", k, table[k].client[m]);
	if(table[k].client[m]>0) write(table[k].client[m], game_buf, sizeof(game_buf));	
      }
    } /* for */

    /*  Connect new port server when forked it  */
    if(strcmp("FORK", game_buf) == 0){
      //sleep(1);
      s[fork_num] = client_socket_procedure(server,server_port);
      table[fork_num].client[0] = table[0].client[1];
      table[0].client[1] = 0;
      table[fork_num].server = s[fork_num];
      printf("s[%d], table[%d].client[0]=%d", fork_num, fork_num, table[fork_num].client[0]);
      fork_num++;
    } //strcmp
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
  return  proxy_server(server_port, argv[2],client_port);
}
