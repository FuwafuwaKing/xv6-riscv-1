#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_getfdmap(void)
{
    struct proc *p = myproc();
    unsigned short fd_mask = 0;

    for (int fd = 0; fd < NOFILE; fd++) {
        if (p->ofile[fd] != 0) {
            fd_mask |= (1 << fd);
        }
    }

    return fd_mask;
}

uint64 sys_setenv(void) {
  char name[MAX_ENV_NAME];
  char value[MAX_ENV_VALUE];

  if (argstr(0, name, MAX_ENV_NAME) < 0)
    return -1;
  if (argstr(1, value, MAX_ENV_VALUE) < 0)
    return -1;

  return setenv(name, value);
}

uint64 sys_getenv(void) {
  char name[MAX_ENV_NAME];
  uint64 buf;
  int bufsize;

  // ユーザーから渡された環境変数名をカーネル内の name バッファにコピー
  if (argstr(0, name, MAX_ENV_NAME) < 0)
    return -1;

  // ユーザーから渡されたバッファのアドレスを buf にコピー
  argaddr(1, &buf);

  // ユーザーから渡されたバッファのサイズを bufsize にコピー
  argint(2, &bufsize);

  // 追加のエラーチェック
  if (buf == 0 || bufsize <= 0)
    return -1;

  // 指定された環境変数の値を取得
  char *value = getenv(name);
  if (value == 0)
    return -1;

  // 環境変数の値をユーザーバッファにコピー
  if (copyout(myproc()->pagetable, buf, value, bufsize) < 0)
    return -1;

  return 0;
}
