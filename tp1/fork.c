#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  switch(fork()) {
    case 0:
      printf("fils!\n");
      return 1;
    default:
      wait(NULL);
      printf("père!\n");
      return 0;
  }
}
