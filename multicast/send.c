#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main()
{
  int sock;
  struct sockaddr_in addr;
  int n;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  inet_pton(AF_INET, "239.192.1.2", &addr.sin_addr.s_addr);
  while(1){
    n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    sleep(3);
  }
  if (n < 1) {
    perror("sendto");
    return 1;
  }
  close(sock);
  return 0;
}
