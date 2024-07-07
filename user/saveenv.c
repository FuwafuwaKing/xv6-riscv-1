//saveenv.c 環境変数をenvファイルに保存する

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define ENV_FILE "/env"

// 既存の環境変数を読み込む
void load_existing_env(char *buffer, int size) {
  int fd = open(ENV_FILE, O_RDONLY);
  if (fd < 0) {
    // ファイルが存在しない場合は何もしない
    return;
  }

  int n = read(fd, buffer, size - 1);
  if (n >= 0) {
    buffer[n] = '\0';  // 文字列の終端を設定
  } else {
    buffer[0] = '\0';  // 読み込みに失敗した場合は空文字列
  }
  close(fd);
}

// 環境変数を保存する
void save_env(int argc, char *argv[]) {
  char existing_env[1024];
  load_existing_env(existing_env, sizeof(existing_env));

  // 新しい環境変数を保存するファイルを開く（書き込みモード）
  int fd = open(ENV_FILE, O_WRONLY | O_CREATE);
  if (fd < 0) {
    printf("saveenv: cannot open %s\n", ENV_FILE);
    return;
  }

  // 既存の環境変数をファイルに書き込む
  write(fd, existing_env, strlen(existing_env));

  char env_value[128];
  // コマンドライン引数として渡された環境変数をファイルに書き込む
  for (int i = 1; i < argc; i++) {
    if (getenv(argv[i], env_value, sizeof(env_value)) >= 0) {
      // "NAME=VALUE;" の形式でファイルに書き込む（改行を含まない）
      write(fd, argv[i], strlen(argv[i]));
      write(fd, "=", 1);
      write(fd, env_value, strlen(env_value));
      write(fd, ";", 1);  // 改行の代わりにセミコロンを使用
    } else {
      printf("saveenv: environment variable %s not found\n", argv[i]);
    }
  }

  // ファイルを閉じる
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: saveenv <env1> <env2> ...\n");
    exit(1);
  }
  save_env(argc, argv);
  exit(0);
}
