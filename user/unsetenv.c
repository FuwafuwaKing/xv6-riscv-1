//unsetenv(現状、saveenvで保存したファイルを書き換え環境変数を消すもので、プロセス内の環境変数を消すものではない)

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define ENV_FILE "/env"

// 環境変数をファイルから削除する関数
void unset_env(const char *name) {
  char buffer[1024];
  int fd;

  // 既存の環境変数を読み込む
  fd = open(ENV_FILE, O_RDONLY);
  if (fd < 0) {
    printf("unsetenv: cannot open %s\n", ENV_FILE);
    return;
  }

  int n = read(fd, buffer, sizeof(buffer) - 1);
  if (n < 0) {
    printf("unsetenv: read error on %s\n", ENV_FILE);
    close(fd);
    return;
  }
  buffer[n] = '\0';
  close(fd);

  // 新しい環境変数を保存するためにファイルを開く（書き込みモード）
  fd = open(ENV_FILE, O_WRONLY | O_CREATE | O_TRUNC);
  if (fd < 0) {
    printf("unsetenv: cannot open %s\n", ENV_FILE);
    return;
  }

  char *var = buffer;
  char *semicolon;

  // 既存の環境変数をチェックし、削除する名前と一致しないものを保存
  while ((semicolon = strchr(var, ';')) != 0) {
    *semicolon = '\0';
    char *existing_name = var;
    char *existing_value = strchr(var, '=');
    if (existing_value != 0) {
      *existing_value = '\0';
      existing_value++;
      if (strcmp(existing_name, name) != 0) {
        // 名前が一致しない環境変数をファイルに書き込む
        write(fd, existing_name, strlen(existing_name));
        write(fd, "=", 1);
        write(fd, existing_value, strlen(existing_value));
        write(fd, ";", 1);
      }
    }
    var = semicolon + 1;
  }

  // ファイルを閉じる
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <variable>\n", argv[0]);
    exit(1);
  }

  char *var = argv[1];

  // 環境変数を削除
  unset_env(var);

  exit(0);
}
