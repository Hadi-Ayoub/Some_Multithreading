#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;
unsigned int main_sleep_seconds;
unsigned int thr_sleep_seconds;
char main_op;
char thr_op;

void do_sem_op(const char * thr_name, char op) {
  switch (op) {
    case 'p':
      printf("%s: %c\n", thr_name, op);
      if (sem_wait(&sem)) {
        perror("sem_wait");
        exit(1);
      }
      break;
    case 'v':
      printf("%s: %c\n", thr_name, op);
      if (sem_post(&sem)) {
        perror("sem_post");
        exit(1);
      }
      break;
  }
}

void * f(void * arg) {
  if (thr_sleep_seconds > 0) {
    printf("thread fils: je m'endors pour %u secondes\n", thr_sleep_seconds);
    sleep(thr_sleep_seconds);
  }

  do_sem_op("thread fils", thr_op);

  printf("thread fils: je m'arrête\n");
  return NULL;
}
int main(int argc, char* argv[]) {
  if (argc != 6) {
    fprintf(stderr, "usage: thread-sem-testbed <main-sleep-time> <thread-sleep-time> <sem-init-value> <main-op> <thread-op>\n");
    return 1;
  }

  main_sleep_seconds = atoi(argv[1]);
  thr_sleep_seconds = atoi(argv[2]);
  unsigned int sem_init_value = atoi(argv[3]);
  main_op = argv[4][0];
  thr_op = argv[5][0];

  if (sem_init(&sem, 0, sem_init_value)) {
    perror("sem_init");
    exit(1);
  }

  pthread_t thread;
  if (pthread_create(&thread, NULL, f, NULL)) {
    perror("pthread_create");
    exit(1);
  }

  if (main_sleep_seconds > 0) {
    printf("thread principal: je m'endors pour %u secondes\n", main_sleep_seconds);
    sleep(main_sleep_seconds);
  }

  do_sem_op("thread principal", main_op);

  printf("thread principal: j'attends la fin du thread fils\n");
  if (pthread_join(thread, NULL)) {
    perror("pthread_join");
    exit(1);
  }

  printf("thread principal: je m'arrête\n");
  return 0;
}
