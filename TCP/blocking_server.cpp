#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
int deleteClient(int * clients,int clientSize,int client){
    for (int i=client;i<clientSize-1;i++){
            clients[i] = clients[i+1];
    }
    return clientSize-1;
}
int main(){
    //Khởi tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1){
        perror("socket() failed");
        return 1;
    }
    //turn socket to asynchonous mode
    unsigned long ul = 1;
    ioctl(listener,FIONBIO,&ul);

    //tạo cấu trúc địa chỉ socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    if (bind(listener, (struct sockaddr*)&addr,sizeof(addr))){
        perror("bind() failed.");
        return 1;
    }
    if(listen(listener,5)){
        perror("listen() failed");
        return 1;
    }
    char buf[256];
    int clients[64];
    int num_clients = 0;
    while (1){
        char rep[256] = "Received: ";
        // printf("Waiting for new connection.\n");
        int client = accept(listener,NULL,NULL);
        if (client == -1){
            if (errno == EWOULDBLOCK || errno == EAGAIN){
                // error for uncompleted io process

            }
            else{
                perror("accept() failed");
                return 1;
            }
        }
        else{
        printf("New connection is accepted: %d\n",client);
        clients[num_clients++] = client;
        unsigned long ul = 1;
        ioctl(client,FIONBIO,&ul);
        }
        
        
        for (int i=0;i<num_clients;i++){
            int ret = recv(clients[i],buf,sizeof(buf),0);
            if (ret == -1)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {

                }
                else
                {
                    close(clients[i]);
                    continue;
                }
            }
            else if (ret == 0){
                close(clients[i]);
                num_clients = deleteClient(clients,num_clients,i);
                continue;
            }
            else{
                buf[ret] = 0;
                printf("Dữ liệu nhận được từ %d: %s\n",clients[i],buf);
                strcat(rep,buf);
                send(clients[i],rep,sizeof(rep),0);
            }
        }
    }
    close(listener);
    return 0;
}