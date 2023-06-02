#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
void clean_stdin(void) {
  int c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);
}
int *countString(char *str, char *sub, int strSize, int subSize) {
  int s = 0;
  int i = 0;
  int start = 0;
  int *res = (int *)malloc(2 * sizeof(int));
  res[0] = 0;
  res[1] = strSize;
  while (i < strSize) {
    if (str[i] == sub[0]) {
      int j = 1;
      int count = 0;
      start = 0;
      for (j = 1; j < 10; j++) {
        if (i + j >= strSize) {
          res[1] = i;
          i = i + j;
          break;
        } else {
          if (str[i + j] == sub[0]) {
            if (count == 0)
              start = i + j;
            count++;
          }
          if (str[i + j] != sub[j])
            break;
        }
        printf("i+j: %d,j: %d,start: %d\n", i + j, j, start);
      }
      if (j > subSize - 1) {
        i = i + subSize;
        s++;
      } else {
        i = start != 0 ? start : i + 1;
      }
    } else {
      i++;
    }
  }
  res[0] = s;
  return res;
}
int main() {
  // char name[100];
  // char MSSV[100];
  // char msg[256];
  // while (1){
  // strcpy(msg,"");
  // printf("Enter name: ");
  // fgets(name,sizeof(name),stdin);
  // name[strcspn(name,"\n")] = '\0';
  // strcat(msg,name);
  // strcat(msg," ");
  // printf("Enter MSSV: ");
  // fgets(MSSV,sizeof(MSSV),stdin);
  // name[strcspn(MSSV,"\n")] = '\0';
  // strcat(msg,MSSV);
  // printf("MSSV: ");
  // fputs(MSSV,stdout);
  // printf("name: ");
  // fputs(,stdout);
  // fputs(msg,stdout);
  // time_t t = time(NULL);
  // struct tm tm = *localtime(&t);
  // char res[256];
  // snprintf(res,sizeof,"now: %d-%d-%d
  // %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
  // //printf("now: %d-%d-%d
  // %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
  // printf("res: %s",res);
  char str[200] = "1234124341`";
  char sub[100] = "00045";
  snprintf(str, sizeof(str), "%s %s", str, sub);
  printf("%s\n", str);
  // int* res = (int*)malloc(2*sizeof(int));
  // res = countString(str,sub,strlen(str),strlen(sub));
  // printf("str: %d sub: %d\n",strlen(str),strlen(sub));
  // printf("%d %d\n",res[0],res[1]);
  return 0;
}