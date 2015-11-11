#ifndef __COMMON_H_DEFINED__
#define __COMMON_H_DEFINED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      /* read(),write(),fork(),close(),...       */
#include <sys/socket.h>  /* socket(), bind(), listen(), accept()... */
#include <netinet/in.h>  /* struct sockaddr_in,...                  */
#include <arpa/inet.h>   /* inet_ntoa(),....                        */
#include <sys/un.h>      /* struct sockaddr_un                      */
#include <fcntl.h>       /* open(),...                              */
#include <netdb.h>       /* gethostbyname(),....                    */

#include <tcpd.h>        /* hosts_access().....                     */ 
#include <syslog.h>

#include <termios.h>

#define PORT_NO (1235)   /* default port number for INET domain */

int server_socket_procedure(int port);
int client_socket_procedure(char* server,int port);

int is_access_granted(char* daemon, int s);

#endif

