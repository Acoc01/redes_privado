#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct data_t {
  char name[1024];
  size_t size;
};

void send_file(FILE *fp, int sockfd, struct data_t *datos);

int main() {
  char *ip = "127.0.0.1";
  int port = 65432;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;

  FILE *fp;
  struct data_t datos;
  printf("Ingrese nombre del archivo a enviar\n");
  scanf("%s", datos.name);
  fp = fopen(datos.name, "r");
  if (fp == NULL) {
    printf("[-] Error al abrir archivo\n");
    return 0;
  }
  fseek(fp, 0L, SEEK_END);
  datos.size = ftell(fp);
  rewind(fp);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("[-] Error al crear socket\n");
    return 0;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e == -1) {
    printf("[-] Error al conectar\n");
    return 0;
  }
  send_file(fp, sockfd, &datos);
  close(sockfd);
  printf("[+] Conexion cerrada\n");

  return 0;
}

int send_all(int socket, void *buffer, size_t length) {
  char *ptr = (char *)buffer;
  while (length > 0) {
    int i = send(socket, ptr, length, 0);
    if (i < 1)
      return -1;
    ptr += i;
    length -= i;
  }
  return 1;
}

void send_file(FILE *fp, int sockfd, struct data_t *datos) {
  char *data = malloc(datos->size);
  double cont = 0;
  if (send_all(sockfd, datos, sizeof(*datos)) == -1) {
    printf("[-] Error al enviar nombre y tamanio de archivo");
    return;
  }
  fread(data, 1, datos->size, fp);
  if (send_all(sockfd, data, datos->size) == -1) {
    printf("[-] Error al enviar archivo\n");
  }
  fclose(fp);
}
