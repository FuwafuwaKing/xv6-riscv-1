// user/getenv.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 128

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <variable>\n", argv[0]);
    exit(1);
  }

  char *var = argv[1];
  char buf[BUF_SIZE];

  // システムコールを使用して環境変数を取得
  if (getenv(var, buf, sizeof(buf)) < 0) {
    printf("%s not found\n", var);
  } else {
    printf("%s: %s\n", var, buf);
  }

  exit(0);
}
