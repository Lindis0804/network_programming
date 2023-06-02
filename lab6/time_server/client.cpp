#include "../utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  int ret;
  int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(9000);

  if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
    perror("connect() failed");
    return 1;
  }

  // Tao them tien trinh moi, chay 2 cong viec song song
  if (fork() == 0) {
    // Tien trinh con
    // Doc du lieu tu ban phim, gui sang server
    printf("child socket client = %d\n", client);
    char buf[256];
    char msg[256];
    while (1) {
      printf("Input time format: \n");
      fgets(buf, sizeof(buf), stdin);
      buf[strlen(buf) - 1] = 0;
      snprintf(msg, sizeof(msg), "GET_TIME %s", buf);
      ret = send_stream_msg(client, msg, strlen(msg), BUFF_SIZE);
      if (strncmp(buf, "exit", 4) == 0)
        break;
    }
  } else {
    // Tien trinh cha
    // Nhan du lieu tu server va in ra man hinh
    printf("main socket client = %d\n", client);
    char buf[256];
    while (1) {
      ret = recv_stream(client, buf, sizeof(buf), BUFF_SIZE);
      if (ret <= 0)
        break;
      // buf[ret] = 0;
      printf("Received: %s\n", buf);
    }
  }

  // Ket thuc, dong socket
  close(client);

  // Ket thuc cac tien trinh dang chay
  killpg(0, SIGKILL);
  return 0;
}