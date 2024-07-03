#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// バッファのサイズ
#define MAXPATH 128

int main(void) {
  char path[MAXPATH];
  char buf[MAXPATH];
  struct stat st;
  struct dirent de;
  int fd, parent_fd;
  int n;

  // ルートディレクトリの場合は直接返す
  if (stat(".", &st) < 0) {
    printf("pwd: cannot stat current directory\n");
    exit(1);
  }
  if (st.ino == ROOTINO) {
    printf("/\n");
    exit(0);
  }

  path[0] = '\0';

  while (1) {
    if ((fd = open(".", O_RDONLY)) < 0) {
      printf("pwd: cannot open current directory\n");
      exit(1);
    }

    if (fstat(fd, &st) < 0) {
      printf("pwd: cannot stat current directory\n");
      close(fd);
      exit(1);
    }

    if (st.ino == ROOTINO) {
      close(fd);
      break;
    }

    if ((parent_fd = open("..", O_RDONLY)) < 0) {
      printf("pwd: cannot open parent directory\n");
      close(fd);
      exit(1);
    }

    if (chdir("..") < 0) {
      printf("pwd: cannot change directory\n");
      close(fd);
      close(parent_fd);
      exit(1);
    }

    while (read(parent_fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == st.ino) {
        break;
      }
    }

    if (de.inum == st.ino) {
      // path の内容を buf にコピー
      for(n = 0; (buf[n] = path[n]); n++)
        ;
      // path に "/" をコピー
      n = 0;
      path[n++] = '/';
      // path にディレクトリ名をコピー
      for(int i = 0; de.name[i]; i++)
        path[n++] = de.name[i];
      path[n] = '\0';
      // path に buf の内容を結合
      for(int i = 0; buf[i]; i++)
        path[n++] = buf[i];
      path[n] = '\0';
    } else {
      printf("pwd: cannot find directory entry\n");
      close(fd);
      close(parent_fd);
      exit(1);
    }

    close(fd);
    close(parent_fd);
  }

  printf("%s\n", path);
  exit(0);
}
