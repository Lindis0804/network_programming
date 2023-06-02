#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
using namespace std;
int main(){
    //tạo socket
    int listener = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (listener != -1){
        printf("Socket created: %d\n",listener);
    }
    else{
        printf("Failed to create socket.\n");
        exit(1);
    }
    //khai báo cấu trúc địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    //gán địa chỉ với socket
    if (bind(listener,(struct sockaddr *)&addr,sizeof(addr))){
        printf("bind() failed.\n");
        exit(1);
    }
    //lắng nghe kết nối
    if (listen(listener,5)){
        printf("listen() failed.\n");
        exit(1);
    }
    printf("Waiting for a new client....\n");

    //chấp nhận kết nối
    int client = accept(listener,NULL,NULL);
    if (client == -1){
        printf("Accept failed.\n");
        exit(1);
    }
    printf("New client connected: %d\n",client);

    //nhận dữ liệu từ client
    char buf[256];
    int ret = recv(client,buf,sizeof(buf),0);
    if (ret <=0 ){
        printf("recv() failed.\n");
        exit(1);
    }

    //thêm kí tự kết thúc xâu và in ra màn hình
    if (ret < sizeof(buf)){
        buf[ret] = 0;
        puts(buf);
    }
    //gửi dữ liệu sang client
    send(client,buf,strlen(buf),0);
    // đóng kết nối
    close(client);
    close(listener);
    return 0;
    }