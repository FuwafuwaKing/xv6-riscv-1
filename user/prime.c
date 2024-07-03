#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter(int p[2]) {
  close(p[1]);
  int prime;
  if(read(p[0], &prime, sizeof(int)) == 0) {
    close(p[0]);
    exit(0);
  }
  printf("Create %d\n", prime);
  
  int newp[2];
  pipe(newp);

  if(fork() == 0) {
    close(p[0]);
    filter(newp);
  } else {
    close(newp[0]);
    int num;
    while(read(p[0], &num, sizeof(int)) != 0) {
      if(num % prime != 0) {
        write(newp[1], &num, sizeof(int));
      }
    }
    close(p[0]);
    close(newp[1]);
    wait(0);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "Usage: prime [number]\n");
    exit(1);
  }

  int max = atoi(argv[1]);
  if (max < 2) {
    fprintf(2, "2より小さい数を入力してください\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  if(fork() == 0) {
    filter(p);
  } else {
    close(p[0]);// 親プロセスのパイプ読み取り側を閉じる
    for(int i = 2; i <= max; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);// パイプの書き込み側を閉じる
    wait(0);//まつ
  }
  exit(0);
}
