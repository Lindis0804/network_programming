#include "../utils.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void *client_thread(void *);
char **using_accounts;
int num_of_using_accounts = 0;
int clients[256];
int num_of_clients = 0;
char account_file[100] = "account.txt";
int ret;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void process_request(int client, char **using_accounts,
                     int &num_of_using_accounts, char *buf, int buf_size,
                     bool &has_signed_in, char *execute_file,
                     char account_file[]) {
  char username[256];
  char password[256];
  printf("file name: %s\n", execute_file);
  if (!has_signed_in) {
    // chua dang nhap
    ret = sscanf(buf, "%s%s", username, password);

    /* khóa biến mutex lại tránh các đụng độ tài nguyên dùng chung*/
    pthread_mutex_lock(&mutex);
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

    /* Mở khóa biến mutex*/
    pthread_mutex_unlock(&mutex);
    ret = send_stream_msg(client, buf, strlen(buf), 0, BUFF_SIZE);
  } else {
    // da dang nhap
    char command[256];
    snprintf(command, sizeof(command), "%s > %s", buf, execute_file);
    printf("command: %s\nbuf: %s\n execute_file:%s\n", command, buf,
           execute_file);
    int res = system(command);
    printf("res: %d\n", res);
    if (res == 0)
      ret = send_file(client, execute_file, BUFF_SIZE);
    else {
      strcpy(buf, "Execute command fail.");
      printf("Send: %s\n", buf);
      ret = send_stream_msg(client, buf, strlen(buf), 0, BUFF_SIZE);
    }
  }
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

  while (1) {
    int client = accept(listener, NULL, NULL);
    if (client == -1) {
      perror("accept() failed");
      continue;
      ;
    }
    printf("New client connected: %d\n", client);

    pthread_t thread_id;

    // tạo một luồng mới
    pthread_create(&thread_id, NULL, client_thread, &client);

    // đánh dấu luồng ở trạng thái tách rời, khi luồng kết thúc ở trạng thái này
    // các tài nguyên của luồng tự giải phóng
    pthread_detach(thread_id);
  }

  close(listener);

  return 0;
}

void *client_thread(void *param) {
  int client = *(int *)param;
  char buf[256];
  bool has_signed_in = false;
  int thread_id = pthread_self();
  char execute_file[256];
  snprintf(execute_file, sizeof(execute_file), "./execute_file/%d.txt",
           thread_id);
  while (1) {
    int ret = recv_stream(client, buf, sizeof(buf), 0, BUFF_SIZE);
    if (ret <= 0)
      break;
    // buf[ret] = 0;
    printf("Received from %d: %s\n", client, buf);
    process_request(client, using_accounts, num_of_using_accounts, buf,
                    sizeof(buf), has_signed_in, execute_file, "account.txt"

    );
  }

  close(client);
}