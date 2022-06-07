#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void recv_file(int sockfd, int size, char *name);

int main(){
    char *ip_addr = "127.0.0.1";
    int port = 65432;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd = 0){
        printf("[-] Error al crear socket\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0){
        printf("[-] Error al bindiar socket\n");
        return;
    }

    e = listen(sockfd, 10);
    if(e != 0){
        printf("[-]Error al escuchar\n");
        return;
    }
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    int aux,filesize;
    char filename[1024];
    aux = recv(sockfd, filename, 1024, 0);
    if(aux <= 0){
        printf("[-] Error al recibir nombre de archivo\n");
        return;
    }
    aux = recv(sockfd, filesize, 1024, 0);
    if(aux <= 0){
        printf("[-] Error al recibir tamanio del archivo\n");
        return;
    }
    write_file(new_sock, filesize, filename);
}

void recv_file(int sockfd, int size, char *name){
    int n;
    double cont = 0;
    FILE *fp;
    char buffer[1024];
    fp = fopen(name, "w");
    if(fp == NULL){
        printf("[-] Error al crear archivo");
        return;
    }

    while(1){
        n = recv(sockfd, buffer, 1, 0);
        if(n <= 0){
            break;
            return;
        }
        cont++;
        printf("[+] %lf porciento se ha recibido...\n",(cont/size)*100));
        fprintf(fp, "%s", buffer);
        bzero(buffer, 1024);
    }
    return;
}