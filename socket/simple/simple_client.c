#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      /* read(),write(),fork(),close(),...       */
#include <sys/socket.h>  /* socket(), bind(), listen(), accept()... */
#include <netinet/in.h>  /* struct sockaddr_in,...                  */
#include <netdb.h>       /* gethostbyname(),....                    */

#define PORT  (23456)

int main(int argc, char* argv[])
{
  int s;
  struct sockaddr_in  addr;
  struct hostent*     hp;
  char buf[BUFSIZ];

  if( argc<2 ) {
    printf("%s SERVER\n",argv[0]);
    exit(1);
  }
  
  if( (s=socket(AF_INET, SOCK_STREAM, 0)) <0 ) {
    perror("socket");    exit(1);
  }

  memset((char*)&addr, 0, sizeof(addr));

  if( (hp=gethostbyname(argv[1]))==NULL) {
    perror("gethostbyname");    exit(1);
  }

  bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(PORT);

  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0){
    perror("connect");    exit(1);
  }
  
  read(s, buf, sizeof(buf));
  printf("%s", buf);

  close(s);
  return 0;
}

