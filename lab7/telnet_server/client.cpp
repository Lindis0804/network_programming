#include "../utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void *thread_proc(void *);
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
  pthread_t tid;
  pthread_create(&tid, NULL, thread_proc, &client);

  char buf[256];
  while (1) {
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = 0;
    ret = send_stream_msg(client, buf, strlen(buf), 0, BUFF_SIZE);
    if (strncmp(buf, "exit", 4) == 0)
      break;
  }

  // Ket thuc, dong socket
  close(client);

  return 0;
}

void *thread_proc(void *param) {
  int client = *(int *)param;
  char buf[256];

  while (1) {
    int ret = recv_stream(client, buf, sizeof(buf), 0, BUFF_SIZE);
    if (ret <= 0)
      break;

    // buf[ret] = 0;
    printf("Received: %s\n", buf);
  }

  close(client);
}