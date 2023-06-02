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
void clean_stdin(void){
    int c;
    do{
        c = getchar();
    }while (c!='\n' && c!=EOF);
}
int main(int argc,char* argv[]){
    char ipAddr[20];
    char MSSV[10];
    char name[100];
    char dateOfBirth[100];
    float GPA;
    char strGPA[5];
    char msg[256];
    char sep[5] = "\r\n" ;
    char buf[256];
    char c;
    strcpy(ipAddr,argv[1]);
    int port;
    sscanf(argv[2],"%d",&port);
    //initialize socket
    int client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (client == -1){
        printf("socket() failed.");
        return 1;
    }
    //khai báo cấu trúc địa chỉ client
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddr);
    addr.sin_port = htons(port);

    //kết nối đến server
    int ret = connect(client,(struct sockaddr *)&addr,sizeof(addr));
    if (ret == -1){
        perror("connect() failed.");
        return 1;
    }
    //nhận dữ liệu từ server
    ret = recv(client,buf,sizeof(buf),0);
    if(ret <=0){
        printf("Connection closed\n");
        return 1;
    }
    if (ret < sizeof(buf)){
        buf[ret] = 0;
    }
    printf("%d bytes received: %s\n",ret,buf);

    //gửi dữ liệu đến server
    // getchar();
        // cout<<"c_birth"<<endl;
        // puts(dateOfBirth);
        // while ((c=getchar())!='\n' && c!=EOF);
    while (1){
        strcpy(msg,"MSSV: ");
        printf("Enter MSSV: ");

        fgets(MSSV,sizeof(MSSV),stdin);
        MSSV[strcspn(MSSV,"\n")] = '\0';

        printf("Enter name: ");
        fgets(name,sizeof(name),stdin);
        name[strcspn(name,"\n")] = '\0';

        printf("Enter dateOfBirth: ");
        fgets(dateOfBirth,sizeof(dateOfBirth),stdin);
        dateOfBirth[strcspn(dateOfBirth,"\n")] = '\0';
        printf("Enter GPA: ");
        scanf("%f",&GPA);
        snprintf(strGPA,sizeof(strGPA),"%f",GPA);
        // strcat(msg,MSSV);
        // strcat(msg,sep);
        // strcat(msg,"Name: ");
        // strcat(msg,name);
        // strcat(msg,sep);
        // strcat(msg,"DateOfBirth: ");
        // strcat(msg,dateOfBirth);
        // strcat(msg,sep);
        // strcat(msg,"GPA: ");
        // strcat(msg,strGPA);
        snprintf(msg,sizeof(msg),"%s %s %s %.2f",MSSV,name,dateOfBirth,GPA);
        printf("msg: %s\n",msg);
        send(client,msg,strlen(msg),0);
        clean_stdin();
    }
    close(client);
    return 0;

}