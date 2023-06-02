#include "./utils.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int main() {
  char msg[256];
  strcpy(msg, "GET_TIME hhhhhhh");
  char buf[256];
  strcpy(buf, sub_str(msg, strlen(msg), 9, -1));
  printf("buf: %s\n", buf);
  return 0;
}