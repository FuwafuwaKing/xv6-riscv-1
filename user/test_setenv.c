// user/test_setenv.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <variable> <value>\n", argv[0]);
    exit(1);
  }

  char *var = argv[1];
  char *value = argv[2];

  if (setenv(var, value) < 0) {
    printf("setenv failed\n");
    exit(1);
  }
  printf("setenv succeeded: %s=%s\n", var, value);
  exit(0);
}
