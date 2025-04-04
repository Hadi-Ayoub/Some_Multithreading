#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t shared_counter = 0;
uint32_t nb_incr_per_thread = 0;

void * f(void * arg) {
  (void) arg;
  for (uint32_t i = 0; i < nb_incr_per_thread; ++i) {
    shared_counter += 1;
  }
  return NULL;
}
int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: para-write <nb-threads> <nb-incr>\n");
    return 1;
  }

  uint32_t nb_threads = atoi(argv[1]);
  uint32_t nb_incr = atoi(argv[2]);

  assert(nb_threads > 0);
  assert(nb_incr % nb_threads == 0);
  nb_incr_per_thread = nb_incr / nb_threads;

  printf("%u threads, chaque thread fait %u +1\n", nb_threads, nb_incr_per_thread);
  printf("valeur attendue : %u\n", nb_incr);

  pthread_t threads[nb_threads];
  for (uint32_t i = 0; i < nb_threads; ++i) {
    if (pthread_create(&threads[i], NULL, f, NULL)) {
      perror("pthread_create");
      exit(1);
    }
  }

  for (uint32_t i = 0; i < nb_threads; ++i) {
    if (pthread_join(threads[i], NULL)) {
      perror("pthread_join");
      exit(1);
    }
  }

  printf("valeur calculée : %u\n", shared_counter);
  return 0;
}
