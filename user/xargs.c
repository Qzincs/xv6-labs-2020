#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(2, "Usage: xargs command initial-args\n");
    exit(1);
  }

  char c;               // read读取的缓冲区
  char buf[100];        // 参数缓冲区
  char *args[MAXARG];   // 新参数列表
  int bufn = 0;         // 指示buf的末尾
  int argsn = argc - 1; // 指向参数列表的末尾

  // 先将 command 和 initial-args 写入参数列表，最后一个初始参数的位置是args[argc-2]
  for (int i = 0; i + 1 < argc; i++)
  {
    args[i] = argv[i + 1];
  }

  // 从标准输入读取参数，并执行命令
  while (read(0, &c, 1))
  {
    // 如果读到'\n'，说明所有参数已读取完毕，应该执行命令
    if (c == '\n')
    {
      // 避免写入空参数
      if (bufn > 0)
      {
        // 最后一个参数存入参数列表
        buf[bufn] = '\0';
        args[argsn] = (char *)malloc(sizeof(buf));
        strcpy(args[argsn], buf);
        argsn++;
        bufn = 0;
      }
      args[argsn++] = 0; // 标志参数列表结束

      if (fork() == 0)
      {
        exec(args[0], args);
        printf("exec failed\n");
      }
      else
      {
        wait(0);
        // 执行完成后重置参数列表，以便执行下一行
        memset(args + argc - 1, 0, argsn);
        argsn = argc - 1;
      }
    }
    // 如果读到空格，则当前参数读取完毕，存入参数列表，将buf重置
    else if (c == ' ')
    {
      // 跳过连续空格
      if (bufn > 0)
      {
        buf[bufn] = '\0';
        args[argsn] = (char *)malloc(sizeof(buf));
        strcpy(args[argsn], buf);
        argsn++;
        bufn = 0;
      }
    }
    else
    {
      buf[bufn++] = c;
    }
  }
  exit(0);
}