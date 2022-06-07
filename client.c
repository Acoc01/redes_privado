#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void send_file(FILE *fp, int sockfd);

int main(){
    char *ip = "127.0.0.1";
    int port = 65432;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;

    FILE *fp;
    char filename[1024];
    int filesize = 0;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("[-] Error al abrir archivo\n");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    printf("[+] Ingrese nombre del archivo a enviar\n");
    scanf("%s",filename);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("[-] Error al crear socket\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_Addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (strict sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1){
        printf("[-] Error al conectar\n");
        return;
    }
    if(send(sockfd, filename, sizeof(filename), 0) == -1){
        printf("[-] Error al enviar nombre de archivo\n");
        return;
    }
    if(send(sockfd, filesize, sizeof(filesize), 0) == -1){
        printf("[-] Error al enviar tamanio de archivo\n");
        return;
    }
    send_file(fp, sockfd);
    close(sockfd);
    printf("[+] Conexion cerrada\n");

    return 0;
}


void send_file(FILE *fp, int sockfd, int size){
    char data[1024];
    double cont = 0;
    while(fgets(data, 1, fp) != NULL){
        if(send(sockfd, data, sizeof(data), 0) == -1){
            printf("[-] Error al enviar archivo\n");
            return;
        }
        printf("[+] %lf porciento enviado...\n",(cont/size)*100);
        bzero(data, 1024);
        cont++;
    }
}