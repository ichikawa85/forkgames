#include "common.h"
#include <time.h>
#include <sys/time.h>

int count = 0;

static int copy_server(int port)
{  
  int s1;
  int s1_a;
  struct sockaddr_in addr;
  //int len = sizeof(addr);
  socklen_t len = sizeof(addr);
  char buf[BUFSIZ];
  char str1[128]={0};  

  s1 = server_socket_procedure(port);

  s1_a = accept(s1, (struct sockaddr*)&addr, &len);
    
  /* if(( s2=accept(s1, (struct sockaddr*)&addr, &len))<0) { */
  /*   perror("accept"); */
  /*   return 1; */
  /* } */

  if( ! is_access_granted("copy_server", s1_a)  ) {
    printf("Access from %s : denied\n", inet_ntoa(addr.sin_addr));
    close(s1_a);
    //continue;
    return 1;
  }

  //  close(s1);    
  //printf("Request from %s\n", inet_ntoa(addr.sin_addr));
  
  while(1){
    sprintf(str1,"%d",count);
    strcpy(buf, str1);
    printf("count: %s\n", buf);
    write(s1_a, buf, sizeof(buf));
    count++;
    sleep(3);
  }
  exit(0);
  close(s1_a);
  return 0;
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

