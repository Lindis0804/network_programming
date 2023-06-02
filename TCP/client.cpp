#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
using namespace std;
int main(){
    //Khai báo socket
    int client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //khai báo địa chỉ của server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    //kết nối đến server
    int res = connect(client,(struct sockaddr*)&addr,sizeof(addr));
    if (res == -1){
        printf("Cannot connect to server.");
        return 1;
    }
    printf("Connect to server successfully.");
    //send message to server
    char *msg = "Hello server";
    send(client,msg,strlen(msg),0);

    //receive message from server
    char buf[2048];
    int len = recv(client,buf,sizeof(buf),0);
    buf[len] = 0;
    printf("Data received: %s\n",buf);

    //end, close socket
    close(client);
    return 0;
}