#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
  int i;

  for(i = 1; i < argc; i++){
    if (argv[i][0] == '$') {
      // 引数が$から始まったら
      // 環境変数名を取得
      char *env_var_name = argv[i] + 1;
      char env_var_value[128];
      if (getenv(env_var_name, env_var_value, sizeof(env_var_value)) < 0) {
        // 環境変数が存在しない場合
        write(1, argv[i], strlen(argv[i]));
      } else {
        // 環境変数の値を出力
        write(1, env_var_value, strlen(env_var_value));
      }
    } else {
      // 引数が$から始まらないなら
      // 通常の文字列を出力
      write(1, argv[i], strlen(argv[i]));
    }
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}