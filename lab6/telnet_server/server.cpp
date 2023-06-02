#include "../utils.h"
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
#include <unistd.h>
char **using_accounts;
int num_of_using_accounts = 0;
int clients[256];
int num_of_clients = 0;
char username[256];
char password[256];
char account_file[100] = "account.txt";
int ret;
void process_request(int client, char **using_accounts,
                     int &num_of_using_accounts, char *buf, int buf_size,
                     bool &has_signed_in, char *execute_file) {
  if (!has_signed_in) {
    // chua dang nhap
    ret = sscanf(buf, "%s%s", username, password);
    if (check_account(username, password, account_file)) {
      if (check_using_account(username, using_accounts,
                              num_of_using_accounts)) {
        strcpy(buf, "Account is using.");
      } else {
        strcpy(using_accounts[num_of_using_accounts], username);
        num_of_using_accounts++;
        has_signed_in = true;
        strcpy(buf, "Sign in successfully.");
      }

    } else
      strcpy(buf, "Username or password is not correct.");
    ret = send_stream_msg(client, buf, strlen(buf), BUFF_SIZE);
  } else {
    // da dang nhap
    char command[256];
    snprintf(command, sizeof(command), "%s > %s", buf, execute_file);
    int res = system(command);
    if (res == 0)
      ret = send_file(client, execute_file, BUFF_SIZE);
    else {
      strcpy(buf, strerror(errno));
      ret = send_stream_msg(client, buf, strlen(buf), BUFF_SIZE);
    }
  }
}
// được gọi khi tín hiệu SIGCHLD ( tín hiệu báo process con kết thúc)
void signalHandler(int signo) {
  int pid = wait(NULL);
  printf("Child %d terminated.\n", pid);
}

int main() {
  using_accounts = (char **)malloc(256 * sizeof(char *));
  for (int i = 0; i < 256; i++)
    using_accounts[i] = (char *)malloc(256 * sizeof(char));
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
  // gọi để xử lý khi sự kiện SIGCHLD happen
  signal(SIGCHLD, signalHandler);

  while (1) {
    printf("Waiting for new client...\n");
    int client = accept(listener, NULL, NULL);
    int pro = fork();
    if (pro == 0) {
      // Tien trinh con
      close(listener);

      // Xu ly ket noi tu client
      int pid = getpid();
      char buf[256];
      char execute_file[256];
      bool has_signed_in = false;
      snprintf(execute_file, sizeof(execute_file), "./execute_file/%d.txt",
               pid);
      while (1) {
        int ret = recv_stream(client, buf, sizeof(buf), BUFF_SIZE);
        if (ret <= 0)
          break;
        printf("Process %d\nReceived from %d: %s\n", pid, client, buf);
        process_request(client, using_accounts, num_of_using_accounts, buf,
                        strlen(buf), has_signed_in, execute_file);
      }

      close(client);
      exit(0);
    }

    // Tien trinh cha
    close(client);
  }

  close(listener);

  return 0;
}