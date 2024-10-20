#include "kernel/types.h"

#include "user/user.h"

void sieve(int *fd) {
  close(fd[1]);
  int num;
  if (read(fd[0], &num, sizeof(int)) == 0) {
    close(fd[0]);
    exit(0);
  }
  printf("prime %d\n", num);
  int child[2];
  if (pipe(child) < 0) {
    fprintf(2, "create pipe error\n");
    exit(1);
  }
  int pid = fork();
  if (pid == 0) {
    sieve(child);
  } else {
    close(child[0]);
    int n;
    while (read(fd[0], &n, sizeof(int)) > 0) {
      if (n % num != 0) {
        write(child[1], &n, sizeof(int));
      }
    }
    close(child[1]);
    close(fd[0]);
    wait(0);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }
  int pid = fork();
  if (pid == 0) {
    sieve(fd);
  } else {
    close(fd[0]);
    for (int i = 2; i <= 35; ++i) {
      write(fd[1], &i, sizeof(int));
    }
    close(fd[1]);
    wait(0);
  }
}