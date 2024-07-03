#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void print_fd_mask(unsigned short fd_mask) {
    char mask_str[17];
    for (int i = 0; i < 16; i++) {
        mask_str[15 - i] = (fd_mask & (1 << i)) ? '1' : '0';
    }
    mask_str[16] = '\0';
    printf("%s\n", mask_str);
}

int main(void)
{
    // テスト用(2つひらいてみる)
    int fd1 = open("testfile1", O_CREATE | O_RDWR);
    int fd2 = open("testfile2", O_CREATE | O_RDWR);

    unsigned short fd_mask = getfdmap();

    print_fd_mask(fd_mask);

    // 開いたファイルを閉じる
    close(fd1);
    close(fd2);

    exit(0);
}
