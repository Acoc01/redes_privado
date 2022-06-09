#ifndef TCP_H
#define TCP_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct data_t {
  char name[1024];
  size_t size;
};

int send_all(int socket, void *buffer, size_t length);
void send_file(FILE *fp, int sockfd, struct data_t *datos);

int recv_all(int socket, void *buffer, size_t length);
void recv_file(int sockfd);

void encrypt_file(char *key_file, char *in_file, char *out_file);
void decrypt_file(char *key_file, char *in_file, char *out_file);

#endif
