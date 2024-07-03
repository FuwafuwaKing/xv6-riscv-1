// user/test_getenv.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <variable>\n", argv[0]);
    exit(1);
  }

  char *var = argv[1];
  char *value = getenv(var);

  if (value) {
    printf("%s: %s\n", var, value);
  } else {
    printf("%s not found\n", var);
  }
  exit(0);
}
