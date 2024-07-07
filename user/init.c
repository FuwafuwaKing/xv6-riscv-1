// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };
#define ENV_FILE "/env"

// 環境変数をファイルから読み込む関数
void load_env() {
  // 環境変数を保存したファイルを開く（読み込みモード）
  int fd = open(ENV_FILE, O_RDONLY);
  if (fd < 0) {
    printf("init: cannot open %s\n", ENV_FILE);
    return;
  }

  char buffer[1024];
  int n = read(fd, buffer, sizeof(buffer) - 1);
  if (n < 0) {
    printf("init: read error on %s\n", ENV_FILE);
    close(fd);
    return;
  }
  buffer[n] = '\0';  // 文字列の終端を設定

  char *var = buffer;
  char *semicolon;

  // セミコロン区切りで各環境変数を処理
  while ((semicolon = strchr(var, ';')) != 0) {
    *semicolon = '\0';  // 環境変数の終端を設定
    char *name = var;
    char *value = strchr(var, '=');
    if (value != 0) {
      *value = '\0';  // 名前と値を分割
      value++;
      setenv(name, value);  // 環境変数を設定
    }
    var = semicolon + 1;  // 次の環境変数に進む
  }

  // ファイルを閉じる
  close(fd);
}

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  // 環境変数をロードする
  load_env();

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }

    for(;;){
      // this call to wait() returns if the shell exits,
      // or if a parentless process exits.
      wpid = wait((int *) 0);
      if(wpid == pid){
        // the shell exited; restart it.
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        exit(1);
      } else {
        // it was a parentless process; do nothing.
      }
    }
  }
}
