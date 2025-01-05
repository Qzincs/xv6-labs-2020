#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int prime; // 接收到的第一个数必是质数
    int num; // 缓冲区
    int p1[2]; // 和左侧的管道
    int p2[2]; // 和右侧的管道
    pipe(p1);
    pipe(p2);

    for(int i = 2; i < 36; i++){
        write(p1[1], &i, sizeof(i));
    }
    close(p1[1]);

    prime = 2;
    // 父进程不会执行，子进程会执行一次
    while(fork()==0) {
        p1[0] = p2[0];
        p1[1] = p2[1];
        close(p2[1]);  // 替换掉的文件描述符依然存在，需要先手动关闭
        pipe(p2);

        // 最后一个进程读不到数据，不创建子进程
        if(read(p1[0], &num, sizeof(num))){
            prime = num;
            printf("prime %d\n", prime);
        } else {
            exit(0);
        }
    }

    while(read(p1[0], &num, sizeof(num))){
        if (num % prime != 0){
            write(p2[1], &num, sizeof(num));
        }    
    }
    
    close(p2[1]);
    wait(0);
    exit(0);
}
