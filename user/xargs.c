#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 标准输入
// ./a/b
// ./c/b
// ./b
// 要运行
// grep ./a/b
// grep ./c/b
// grep ./b

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command args\n");
        exit(1);
    }

    char c;         // read读取的缓冲区
    int cursor = 0; // 指示buf位置
    char buf[100];
    // 从标准输入读取参数，并执行命令
    while (read(0, &c, 1))
    {
        if (c == '\n')
        {
            // argc-1个原参数，一个新参数和一个终止参数0，argv[argc]是第一个原参数
            int argcn = argc - 1 + 2;
            char *args[argcn];

            // 先将xargs后的参数写入参数列表
            for (int i = 0; i + 1 < argc; i++)
            {
                args[i] = argv[i + 1];
                // printf("args[%d]: %s,", i, args[i]);
            }
            buf[cursor] = '\0';
            args[argcn - 2] = buf;
            // printf("args[%d]: %s,", argcn - 2, args[argcn - 2]);
            args[argcn-1] = 0;
            // printf("args[%d]: %s.\n", argcn-1, args[argcn-1]);

            if (fork() == 0)
            {
                exec(args[0], args);
                printf("exec failed\n");
            }
            else
            {
                cursor = 0;
                wait(0);
            }
        }
        else
        {
            buf[cursor++] = c;
        }
    }
    exit(0);
}