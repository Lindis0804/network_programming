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
const int BUFF_SIZE = 3;
const int MAX_SIZE = 256;
// TODO: get sub string
inline char *sub_str(char *str, int len, int start, int end) {
  int x = start;
  int y = end;
  if (len - 1 < y || y == -1)
    y = len - 1;
  if (x < 0)
    x = 0;
  char *temp = (char *)malloc((y - x + 1) * sizeof(char));
  for (int i = 0; i <= y - x; i++) {
    temp[i] = str[x + i];
  }
  // TODO: câu lệnh nhằm mục đích cắt các kí tự thừa phía sau
  temp[y - x + 1] = 0;
  return temp;
}
inline int send_stream(int client, char *buf, int buf_size, int BUFF_SIZE) {
  int l;
  char msg[BUFF_SIZE];
  int ret;
  int sret = 0;
  if (buf_size % BUFF_SIZE == 0)
    l = buf_size / BUFF_SIZE;
  else
    l = buf_size / BUFF_SIZE + 1;
  int i = 0;
  for (; i < l; i++) {
    strcpy(msg, sub_str(buf, buf_size, BUFF_SIZE * i, BUFF_SIZE * (i + 1) - 1));
    ret = send(client, msg, BUFF_SIZE, 0);
    sret += ret;
  }
  return sret;
}
inline int send_stream_msg(int client, char *buf, int buf_size, int BUFF_SIZE) {
  int sret = send_stream(client, buf, buf_size, BUFF_SIZE);
  int ret = send(client, "\0", 1, 0);
  return sret;
}
inline int recv_stream(int client, char *buf, int buf_size, int STREAM_SIZE) {
  int ret;
  int sret = 0;
  char stream[256];
  strcpy(buf, "");
  while (1) {
    ret = recv(client, stream, STREAM_SIZE, 0);
    stream[ret] = 0;
    sret += ret;
    strcat(buf, stream);
    if (strlen(stream) <= 0)
      break;
  }
  return sret;
}
inline int send_file(int client, char *filename, int BUFF_SIZE) {
  FILE *ptr;
  ptr = fopen(filename, "r");
  char str[256];
  char buf[256];
  int ret;
  int sret = 0;
  while (fgets(str, sizeof(str), ptr)) {
    ret = send_stream(client, str, strlen(str), BUFF_SIZE);
    sret += ret;
  }
  ret = send(client, "\0", 1, 0);
  return sret;
}
inline bool check_account(char *username, char *password, char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "r");
  char str[300];
  char account[300];
  snprintf(account, sizeof(account), "%s %s", username, password);
  while (fgets(str, 100, fptr)) {
    str[strlen(str) - 1] = 0;
    if (!strcmp(str, account))
      return true;
  }
  return false;
}
inline bool check_using_account(char *username, char **using_usernames,
                                int &size_of_using_usernames) {
  for (int i = 0; i < size_of_using_usernames; i++) {
    if (!strcmp(username, using_usernames[i]))
      return 1;
  }
  strcpy(using_usernames[size_of_using_usernames], username);
  size_of_using_usernames++;
  return 0;
}
char *format_number(int x) {
  char *msg = (char *)malloc(2 * sizeof(char));
  if (0 <= x && x < 10)
    snprintf(msg, 5, "0%d", x);
  else
    snprintf(msg, 5, "%d", x);
  return msg;
}
