#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    write(p[1], "1234", sizeof("1234"));
    close(p[1]);
    char buf[16];
    int n = read(p[0], buf, sizeof(buf));
    printf("%d: %s\n", n, buf);
    n = read(p[0], buf, sizeof(buf));
    printf("%d: %s\n", n, buf);

    exit(0);
}
