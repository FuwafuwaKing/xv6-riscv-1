#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void test_env() {
  char buf[128];

  // 親プロセスで環境変数を設定
  if (setenv("MYVAR", "Process1") < 0) {
    printf("環境変数の設定に失敗\n");
    exit(1);
  }
  printf("親プロセス: 環境変数 MYVAR=Process1 を設定\n");

  // 親プロセスで環境変数を取得して表示
  if (getenv("MYVAR", buf, sizeof(buf)) < 0) {
    printf("環境変数の取得に失敗\n");
    exit(1);
  }
  printf("親プロセス: 環境変数 MYVAR=%s\n", buf);

  // プロセスをフォーク
  int pid = fork();
  if (pid < 0) {
    printf("フォークに失敗\n");
    exit(1);
  } else if (pid == 0) {
    // 子プロセス
    printf("子プロセス: 環境変数 MYVAR を取得\n");
    if (getenv("MYVAR", buf, sizeof(buf)) < 0) {
      printf("子プロセスで環境変数の取得に失敗\n");
      exit(1);
    }
    printf("子プロセス: 継承された環境変数 MYVAR=%s\n", buf);

    // 子プロセスで環境変数を変更
    if (setenv("MYVAR", "Process2") < 0) {
      printf("子プロセスで環境変数の設定に失敗\n");
      exit(1);
    }
    printf("子プロセス: 環境変数 MYVAR=Process2 に変更\n");

    if (getenv("MYVAR", buf, sizeof(buf)) < 0) {
      printf("子プロセスで環境変数の取得に失敗\n");
      exit(1);
    }
    printf("子プロセス: 変更後の環境変数 MYVAR=%s\n", buf);

    exit(0);
  } else {
    // 親プロセスは子プロセスの終了を待つ
    wait(0);

    // 親プロセスで再度環境変数を取得して表示
    printf("親プロセス: 子プロセス終了後に環境変数 MYVAR を再取得\n");
    if (getenv("MYVAR", buf, sizeof(buf)) < 0) {
      printf("親プロセスで環境変数の取得に失敗\n");
      exit(1);
    }
    printf("親プロセス: 子プロセス終了後の環境変数 MYVAR=%s\n", buf);
  }
}

int main() {
  test_env();
  exit(0);
}
