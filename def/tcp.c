#include "tcp.h"
#include "crypto.h"
#include "util.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int recv_all(int socket, void *buffer, size_t length) {
  while (length > 0) {
    int i = recv(socket, buffer, length, 0);
    if (i < 1)
      return 0;
    buffer += i;
    length -= i;
  }
  return 1;
}

void recv_file(int sockfd) {
  int n;
  double cont = 0;
  FILE *fp;
  struct data_t data_recibida;
  n = recv_all(sockfd, &data_recibida, sizeof(data_recibida));
  if (n <= 0) {
    printf("[-] Error al obtener nombre y tamanio\n");
    return;
  }
  char *buffer = malloc(data_recibida.size);
  fp = fopen(data_recibida.name, "w");
  if (fp == NULL) {
    printf("[-] Error al crear archivo");
    return;
  }
  n = recv_all(sockfd, buffer, data_recibida.size);
  char *c = "d";
  decrypt_file("llave.key", data_recibida.name, c);
  if (n <= 0)
    return;
  fwrite(buffer, 1, data_recibida.size, fp);
  fclose(fp);
}

void encrypt_file(char *key_file, char *in_file, char *out_file) {
  if (sodium_init() != 0) {
    fprintf(stderr, "Client could not initialize cryptography\n");
    return;
  }
  unsigned char key[crypto_secretbox_KEYBYTES];
  open_key(key_file, key);

  FILE *f_in;
  if ((f_in = fopen(in_file, "rb")) == NULL) {
    fprintf(stderr, "Client: could not open input file\n");
    return;
  }

  size_t input_len = file_size(f_in);
  unsigned char *input_data = malloc(input_len);

  read_file(f_in, input_data, input_len);
  fclose(f_in);

  struct encrypted_t enc;
  encrypt(key, &enc, input_data, input_len);

  FILE *f_out;
  if ((f_out = fopen(out_file, "wb")) == NULL) {
    fprintf(stderr, "Client: could not open output file\n");
    return;
  }

  write_file(f_out, enc.nonce, sizeof enc.nonce);
  write_file(f_out, enc.msg, enc.len);

  fclose(f_out);
}

void decrypt_file(char *key_file, char *in_file, char *out_file) {
  if (sodium_init() != 0) {
    fprintf(stderr, "Client: could not initialize cryptography\n");
    return;
  }
  unsigned char key[crypto_secretbox_KEYBYTES];
  open_key(key_file, key);

  FILE *f_in;
  if ((f_in = fopen(in_file, "rb")) == NULL) {
    fprintf(stderr, "Client: could not open input file\n");
    return;
  }

  size_t input_len = file_size(f_in);

  struct encrypted_t enc;
  enc.msg = malloc(input_len - sizeof enc.nonce);

  const size_t msg_len = input_len - sizeof enc.nonce;
  const size_t data_len = msg_len - crypto_secretbox_MACBYTES;

  enc.len = msg_len;

  read_file(f_in, enc.nonce, sizeof enc.nonce);
  read_file(f_in, enc.msg, msg_len);

  unsigned char *out = malloc(data_len);
  if (decrypt(key, out, &enc) != 0) {
    fprintf(stderr, "Client: could not decrypt\n");
    return;
  }

  FILE *f_out;
  if ((f_out = fopen(out_file, "wb")) == NULL) {
    fprintf(stderr, "Client: could not open output file\n");
    return;
  }

  write_file(f_out, out, data_len);
  fclose(f_out);
}
