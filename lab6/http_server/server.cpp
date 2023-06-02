#include "../utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listener == -1) {
    perror("socket() failed");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(9000);

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
    perror("bind() failed");
    return 1;
  }

  if (listen(listener, 5)) {
    perror("listen() failed");
    return 1;
  }

  int num_processes = 8;
  char buf[256];
  for (int i = 0; i < num_processes; i++)
    if (fork() == 0) {
      char *request = NULL;
      int size = 0;
      while (1) {
        int client = accept(listener, NULL, NULL);
        int pid = getpid();
        printf("New client connected: %d in process: %d\n", client, pid);

        // receive msg from client
        int ret = recv_stream(client, buf, sizeof(buf), BUFF_SIZE);
        if (ret <= 0) {
          close(client);
          continue;
        }
        if (ret < sizeof(buf))
          buf[ret] = 0;

        // concat msg to request
        request = (char *)realloc(request, size + ret);
        memcpy(request + size, buf, ret);
        size += ret;
        printf("Request: %s\n", request);

        // send data to browser
        strcpy(
            buf,
            "HTTP/1.1 200 OK\r\nContent-Type: "
            "text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>");
        printf("Send: %s\n", buf);
        ret = send_stream_msg(client, buf, strlen(buf), BUFF_SIZE);
        close(client);
      }
    }

  getchar();
  close(listener);

  killpg(0, SIGKILL);

  return 0;
}