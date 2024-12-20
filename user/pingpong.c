#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // 父进程到子进程的管道
    int p1[2];
    // 子进程到父进程的管道
    int p2[2];
    pipe(p1);
    pipe(p2);

    char buf[1];

    if(fork()==0) {
        read(p1[0], buf, 1);
        int pid = getpid();
        printf("%d: received ping\n", pid);
        write(p2[1], "B", 1);
    } else {
        write(p1[1], "A", 1);
        read(p2[0], buf, 1);
        int pid = getpid();
        printf("%d: received pong\n", pid);
    }
    return 0;
}
