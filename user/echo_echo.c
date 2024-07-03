#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char c[512];

int main(void)
{

    int n;
    while ((n = read(0, c, sizeof(c))) > 0)  // 字読み込む
    {
        write(1, c, n);  // 書き込む
    }
    exit(0);
}
