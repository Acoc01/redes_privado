#include "crypto.h"
#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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
  printf("[+] Abriendo archivo...\n");
  printf("[+] Encriptando archivo...\n");
  char *key_name = "llave.key";
  char *out_file = "encriptado";
  encrypt_file(key_name, datos.name, out_file);
  printf("[+]Abriendo archivo encriptado para envio\n");
  fp = fopen(out_file, "r");
  if (fp == NULL) {
    printf("[-] Error al abrir archivo\n");
    return 0;
  }
  printf("[+] Calculando tamanio del archivo encriptado...\n");
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
