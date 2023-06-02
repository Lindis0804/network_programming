#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(9000);

  if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
    perror("connect() failed.");
    return 1;
  }
  char buf[256];
  while (1) {
    printf("Nhập xâu: ");
    fgets(buf, sizeof(buf), stdin);
    send(client, buf, strlen(buf), 0);
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
      break;
    }

    buf[ret] = 0;
    printf("Dữ liệu nhận được: %s\n", buf);
  }
  close(client);
  return 0;
}