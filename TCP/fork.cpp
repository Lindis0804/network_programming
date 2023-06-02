#include <stdio.h>
#include <unistd.h>
int main() {
  int pid = getpid();
  printf("Before FORK, PID = %d\n", pid);
  fork();
  // fork tách chương trình thành 2 tiến trình,mỗi tiến trình thực hiện
  // thực hiện 2 dòng code tiếp theo
  pid = getpid();
  printf("After FORK 1, PID = %d\n", pid);
  fork();
  // fork tách 2 thành 4 tiến trình, mỗi tiến trình thực hiện các
  // dòng code còn lại
  pid = getpid();
  printf("After FORK 2, PID = %d\n", pid);
  printf("Grand end.\n");
  return 0;
}