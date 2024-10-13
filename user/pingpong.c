#include "kernel/types.h"

#include "kernel/stat.h"
#include "user/user.h"

void write_pipe(int pip_fd, char *s, int len) {
  int write_bytes = write(pip_fd, s, len);
  if (write_bytes == -1) {
    printf("error");
  }
  close(pip_fd);
}

void read_pipe(int pip_fd) {
  char buffer[100];
  int read_bytes = read(pip_fd, buffer, sizeof(buffer) - 1);
  if (read_bytes == -1) {
    printf("error");
  }
  close(pip_fd);
}

int main(int argc, char *argv[]) {
  int pipfd[2];
  if (pipe(pipfd) == -1) {
    printf("error\n");
    exit(1);
  }
  int pid = fork();
  char *message = "a";
  int len = strlen(message);
  if (pid == -1) {
    printf("error\n");
    exit(-1);
  } else if (pid == 0) {
    int pid = getpid();
    read_pipe(pipfd[0]);
    printf("%d: received ping\n", pid);
    write_pipe(pipfd[1], message, len);
  } else {
    int pid = getpid();
    write_pipe(pipfd[1], message, len);
    read_pipe(pipfd[0]);
    printf("%d: received pong\n", pid);
  }
}