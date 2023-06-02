#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
using namespace std;
int main(int argc,char* argv[]){
    struct addrinfo *res, *p;
    int ret = getaddrinfo(argv[1],"https",NULL,&res);
    if (ret == -1){
        printf("Failed to get IP\n");
        return 1;
    }
    p = res;
    while (p!=NULL){
        if (p->ai_family == AF_INET){
            printf("Ipv4\n");
            struct sockaddr_in addr;
            memcpy(&addr,p->ai_addr,p->ai_addrlen);\
            //ntoa: change in_addr to Ipv4 string
            printf("IP: %s\n",inet_ntoa(addr.sin_addr));
        }
        else if (p->ai_family == AF_INET6){
            printf("IPv6\n");
            char buf[64];
            struct sockaddr_in6 addr6;
            memcpy(&addr6,p->ai_addr,p->ai_addrlen);
            //ntop: change number to Ipv4 or IPv6 string
            printf("IP: %s\n",inet_ntop(p->ai_family,&addr6.sin6_addr,buf,sizeof(buf)));
        }
        p = p->ai_next;
    }
    freeaddrinfo(res);
    cout<<"He he"<<endl;
    return 0;
}