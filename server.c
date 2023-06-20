#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define maxClient 10

void child_proc(int sock);
struct sending_packet receive_sock(int sock);
//void send_sock(int sock);

struct sending_packet{
    char sender[1024];
    char receiver[1024];
    char msg[1024];
};

int client_sockets[maxClient] = {0};

void main(){
    struct sockaddr_in s_address, c_address;
    int addrlen = sizeof(s_address);
    int check;
    int part_num = 0;



    int s_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_sock_fd == -1){
        perror("socket failed: ");
        exit(1);
    }

    memset(&s_address, '0', addrlen);
    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_address.sin_port = htons(8080);
    check = bind(s_sock_fd, (struct sockaddr *)&s_address, sizeof(s_address));
    if(check == -1){
        perror("bind error : ");
        exit(1);
    }

    printf("<<<< Chat server >>>>\n");
    printf("Server Port : %d\n", 8080);
    printf("Server State : Good\n");

    check = listen(s_sock_fd, 16);
    if(check == -1){
        perror("listen failed: ");
        exit(1);
    }
    while(1){

        int new_sock_fd = accept(s_sock_fd, (struct sockaddr *)&c_address, (socklen_t*)&addrlen);
        if(new_sock_fd<0){
            perror("accept failed: ");
            exit(1);
        }
        part_num++;
        printf("Connected client\n");
        printf("chatter (%d/%d)\n", part_num, maxClient);

        for (int i = 0; i < maxClient; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = new_sock_fd;
                break;
            }
        }


        if(fork()>0){
            close(s_sock_fd);
            child_proc(new_sock_fd);
        }else{
            close(new_sock_fd);
        }
    }
}


void child_proc(int sock){
    struct sending_packet pck;
    while(1){
        if (recv(sock, (struct sending_packet*)&pck, sizeof(pck),0) <= 0){
            //printf("recv error");
            close(sock);
            exit(0);
        }

        printf("%s: %s\n", pck.sender, pck.msg);
        for (int i = 0; i < maxClient; i++) {
            if (client_sockets[i] != 0 && client_sockets[i] != sock) {
                send(client_sockets[i], (struct sending_packet*)&pck, sizeof(pck), 0);
                printf("success!!\n");
            }
        }
        send(sock, (struct sending_packet*)&pck, sizeof(pck), 0);

/*
        pck = receive_sock(sock);
        printf("%s: %s\n", pck.sender, pck.msg);
        if( strcmp(pck.msg,"quit") == 0){
            flag = -1;
        }


        sprintf(pck.msg, "Message received!");
        sprintf(pck.sender, "Server");
        sprintf(pck.receiver, "Client");

        send_sock(sock, pck);
        if(flag == -1){
            break;
        }*/
    }
    shutdown(sock, SHUT_WR);
    exit(0);

}

/*
struct sending_packet receive_sock(int sock){
    struct sending_packet pck;
    recv(sock, (struct sending_packet *)&pck, sizeof(pck),0);
    return pck;
}

void send_sock(int sock,struct sending_packet pck){
    send(sock, (struct sending_packet*)&pck, sizeof(pck), 0);

}*/