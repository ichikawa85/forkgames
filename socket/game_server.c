#include "common.h"
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>

int count = 0;

static int copy_server(int port)
{
  int s1;
  int s1_a;
  struct sockaddr_in addr;
  //int len = sizeof(addr);
  socklen_t len = sizeof(addr);
  char buf[BUFSIZ];
  char read_buf[BUFSIZ];
  char str1[128]={0};

  /* struct list { */
  /*   int sock; */
  /*   char port[BUFSIZ]; */
  /* }; */
  /* struct list table[BUFSIZ]; */

  int select_ret;
  struct timeval t_val = {0, 1000};
  fd_set fds, readfds;

  s1 = server_socket_procedure(port);
  s1_a = accept(s1, (struct sockaddr*)&addr, &len);
    
  if( ! is_access_granted("copy_server", s1_a)  ) {
    printf("Access from %s : denied\n", inet_ntoa(addr.sin_addr));
    close(s1_a);
    //continue;
    return 1;
  }

  int pid;

  /* /\* Non blocking seting *\/ */
  /* int val = 1; */
  /* ioctl(s1_a, FIONBIO, &val); */

  FD_ZERO(&readfds);
  FD_SET(s1_a, &readfds);
  
  while(1){
    memcpy(&fds, &readfds, sizeof(fd_set));
    select_ret = select(s1_a+1, &fds, NULL, NULL, &t_val);
    if(select_ret != 0){
      if(FD_ISSET(s1_a, &fds)){
	read(s1_a,read_buf,BUFSIZ);
	printf("%s\n",read_buf);
	
	switch(atoi(read_buf)){
	case 'a':
	  printf("Press Key %c\n",read_buf);
	  count = count + 10;
	  break;
	case 'f':        /*   fork server   */
	  strcpy(buf, "FORK");
	  printf("%s\n", buf);
	  write(s1_a, buf, sizeof(buf));
	  //*********** FORK() *************
	  if( (pid=fork()) <0 ) {
	    perror("fork");
	    return 1;
	  } else if(pid==0) {
	    close(s1_a);
	    copy_server(12345); //fork at new port
	  }
	  break;
	case 'q':
	  close(s1_a);
	  return 0;
	}
      }
    }else{
      table.sock = 5;
      strcpy(table[fork_num].port, "11133");
      /* sprintf(str1,"%d: %d",port, count); */
      /* strcpy(buf, str1); */
      /* printf("count: %s\n", buf); */
      write(s1_a, reinterpret_cast<char*>(&table), sizeof(buf));
      count++;
      sleep(3);
    }
  }  
  exit(0);
  close(s1_a);
  return 0;
}
  

int main(int argc,char *argv[])
{
  int port = PORT_NO;
  
  if( argc<2 ) {
    port = PORT_NO;
  } else {
    port = atoi(argv[1]);
  }
  printf("port no. = %d\n", port);

  return  copy_server(port);
}

