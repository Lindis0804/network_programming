#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
using namespace std;
int main(int argc,char* argv[]){
    char * ipAddr = new char[20];
    int port;
    printf("%s %s\n",argv[1],argv[2]);
    strcpy(ipAddr,argv[1]);
    sscanf(argv[2],"%d",&port);
    //initialize socket
    int listener  = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (listener == -1){
        printf("socket() failed.");
        return 1;
    }
    /*
    khai báo cấu trúc địa chỉ server
    */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);

    // bind cấu trúc địa chỉ server với socket
    if (bind(listener,(struct sockaddr *)&addr,sizeof(addr))){
        printf("ipAddr: %s port: %d\n",ipAddr,port);
        printf("bind() failed.\n");
        return 1;
    }
    printf("Waiting for new client...\n");
    //lắng nghe kết nối
    if (listen(listener,5)){
        printf("listen() failed.");
        return 1;
    }
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    // chấp nhận kết nối
    int client = accept(listener,(struct sockaddr*)&clientAddr,&clientAddrLen);
    printf("Client IP: %s:%d\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    // gửi đến client
    char * msg = "Hello client\n";
    send(client,msg,strlen(msg),0);

    // nhận từ client
    char buf[256];
    while(1){
        int ret = recv(client,buf,sizeof(buf),0);
        if (ret <= 0){
            printf("Ket noi bi dong");
            break;
        }
        if (ret < sizeof(buf)){
            buf[ret] = 0;
        }
        printf("%d bytes received\n",ret);
        printf("%s",buf);
    
    }
    close(client);
    close(listener);
    delete[] ipAddr;
    return 0;
}