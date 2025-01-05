#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"


/// @brief 在 path 目录及其子目录下查找名为 filename 的文件
/// @param path 搜索目录
/// @param filename 文件名
void 
find(char* path, char* filename) 
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    fprintf(2, "find: path must be a directory\n");
    return;
  
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    // 在原路径后加上/，准备目录下的文件路径
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    // 读取目录下的文件信息
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0){
        continue;
      }
      // DIRSIZ是目录名称的最大长度
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;  // 在目录名超过限制时截断
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      // 是文件则比较文件名
      if (st.type == T_FILE && strcmp(de.name, filename) == 0){
        printf("%s\n", buf);
      }
      // 是目录则递归搜索，不递归 . 和 ..
      else if(st.type == T_DIR){
        if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
          find(buf, filename);
        }
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  // 默认在当前目录找
  if (argc == 2) {
    find(".", argv[1]);
  }
  // 在指定目录找
  else if(argc == 3) {
    find(argv[1], argv[2]);
  } else {
    fprintf(2, "Usage: find path filename\n");
  }
  exit(0);
}
