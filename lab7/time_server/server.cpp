#include "../utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
void *client_thread(void *);
void get_time(int client, char *buf, int buf_size) {
  char *msg = (char *)malloc(256 * sizeof(char));
  time_t t = time(NULL);
  tm tm = *localtime(&t);
  char day[5], month[5];
  int year;
  strcpy(day, format_number(tm.tm_mday));
  strcpy(month, format_number(tm.tm_mon + 1));
  year = tm.tm_year + 1900;
  char *format = NULL;
  format = (char *)realloc(format, buf_size);
  strcpy(format, sub_str(buf, buf_size, 9, -1));
  if (!strncmp(buf, "GET_TIME ", 9)) {
    if (!strcmp(format, "dd/mm/yyyy"))
      snprintf(msg, MAX_SIZE, "%s/%s/%d", day, month, year);
    else if (!strcmp(format, "dd/mm/yy"))
      snprintf(msg, MAX_SIZE, "%s/%s/%d", day, month, year % 100);
    else if (!strcmp(format, "mm/dd/yyyy"))
      snprintf(msg, MAX_SIZE, "%s/%s/%d", month, day, year);
    else if (!strcmp(format, "mm/dd/yy"))
      snprintf(msg, MAX_SIZE, "%s/%s/%d", month, day, year % 100);
    else
      strcpy(msg, "Time format is not correct.");
  } else
    strcpy(msg, "Get time command is not correct.");
  printf("Send: %s\n", msg);
  int ret = send_stream_msg(client, msg, strlen(msg), 0, BUFF_SIZE);
}
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

  while (1) {
    int client = accept(listener, NULL, NULL);
    if (client == -1) {
      perror("accept() failed");
      continue;
      ;
    }
    printf("New client connected: %d\n", client);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, client_thread, &client);
    pthread_detach(thread_id);
  }

  close(listener);

  return 0;
}

void *client_thread(void *param) {
  int client = *(int *)param;
  char buf[256];

  while (1) {
    int ret = recv_stream(client, buf, sizeof(buf), 0, BUFF_SIZE);
    if (ret <= 0)
      break;

    // buf[ret] = 0;
    printf("Received from %d: %s\n", client, buf);
  }

  close(client);
}