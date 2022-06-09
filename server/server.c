#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main() {
  char *ip_addr = "127.0.0.1";
  int port = 65432;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == 0) {
    printf("[-] Error al crear socket\n");
    return 0;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip_addr);

  e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e < 0) {
    printf("[-] Error al bindiar socket\n");
    return 0;
  }

  e = listen(sockfd, 10);
  if (e != 0) {
    printf("[-]Error al escuchar\n");
    return 0;
  }
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
  recv_file(new_sock);
}
