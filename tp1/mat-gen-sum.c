#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t * matrix = 0;
unsigned int n = 0;

void alloc_gen_matrix(unsigned int n) {
  assert(matrix == 0);
  assert(n > 0);

  unsigned int n2 = n * n;
  matrix = malloc(sizeof(uint32_t) * n * n);
  for (unsigned int i = 0; i < n2; ++i) {
    matrix[i] = rand() % 10;
  }
}

uint32_t sum_line(unsigned int line) {
  unsigned int first = n * line;
  unsigned int last = n * (line+1);
  uint32_t sum = 0;
  for (unsigned int i = first; i < last; ++i)
    sum += matrix[i];
  return sum;
}

void free_matrix() {
  free(matrix);
  matrix = 0;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: mat-gen-sum <seed> <n> <nb-threads>\n");
    return 1;
  }

  unsigned int seed = atoi(argv[1]);
  n = atoi(argv[2]);
  unsigned int nb_threads = atoi(argv[3]);

  srand(seed);

  assert(nb_threads == 1);

  alloc_gen_matrix(n);

  uint32_t sum = 0;
  for (unsigned int line = 0; line < n; ++line)
    sum += sum_line(line);
  printf("Somme : %u\n", sum);

  free_matrix();

  return 0;
}
