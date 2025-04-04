#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


uint32_t *matrix = 0;
unsigned int n = 0;

uint32_t global_sum = 0;
//déclaration mutex
pthread_mutex_t sum_mutex; // ******************************
//
typedef struct {
  unsigned int line; // numéro de la ligne
} thread_arg_t;

// fonction inchangée
void alloc_gen_matrix(unsigned int n) {
  assert(matrix == 0);
  assert(n > 0);

  unsigned int n2 = n * n;
  matrix = malloc(sizeof(uint32_t) * n * n);
  for (unsigned int i = 0; i < n2; ++i) {
    matrix[i] = rand() % 10;
  }
}

// fonction inchangée
uint32_t sum_line(unsigned int line) {
  unsigned int first = n * line;
  unsigned int last = n * (line + 1);
  uint32_t sum = 0;
  for (unsigned int i = first; i < last; ++i)
    sum += matrix[i];
  return sum;
}

void free_matrix() {
  free(matrix);
  matrix = 0;
}

// Définition de la fonction exécutée par les threads -> doit respecter l'API de toute fonction exécuté par un thread
// calcul les somme d'une ligne et màj la somme globale
//void *thread_sum(void *arg) {
//  thread_arg_t *my_argument = (thread_arg_t *)arg;
//  unsigned int line = my_argument->line;  //
//
//
//  uint32_t local_sum = sum_line(line);   // utilisation de la fonction sum()
//
//  //  -------------------Mise à jour de la somme globale (section critique) --------------------------
//  pthread_mutex_lock(&sum_mutex);
//  global_sum += local_sum;
//  pthread_mutex_unlock(&sum_mutex);
//
//  free(my_argument); // libération de la mémoire du pointeur my_argument
//  return NULL;
//}

//fonction utilisé par lethread :
void * my_function(void* my_arg){
    thread_arg_t *arg = (thread_arg_t *) my_arg;
    unsigned int line = arg->line;
    uint32_t sum_line = sum(line);
    // beginning of the critical task -> shared resource = global sum
    pthread_mutex_lock(&sum_mutex);
    global_sum += sum_line;
    pthread_mutex_unlock(&sum_mutex);
    // end of the critical task
    free(my_arg);
}


int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: mat-gen-sum <seed> <n> <nb-threads>\n");
    return 1;
  }

  unsigned int seed = atoi(argv[1]);
  n = atoi(argv[2]);
  //*************
  unsigned int nb_threads = atoi(argv[3]);

  if (nb_threads != n) {
    fprintf(stderr, "Error: nb-threads must be equal to n.\n");
    return 1;
  }
  //************

  srand(seed);
  alloc_gen_matrix(n);

  pthread_t threads[nb_threads]; //***********************
  pthread_mutex_init(&sum_mutex, NULL);  //****************


  // boucle de création des threads
  for (unsigned int i = 0; i < nb_threads; ++i) {
    thread_arg_t *arg = malloc(sizeof(thread_arg_t)); // allocation de mémoire pour l'argument du thread
    arg->line = i;

    if (pthread_create(&threads[i], NULL, thread_sum, arg) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  // Attente de la fin des threads
  for (unsigned int i = 0; i < nb_threads; ++i) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      return 1;
    }
  }



  printf("Somme : %u\n", global_sum);

  pthread_mutex_destroy(&sum_mutex); //**********************************
  free_matrix();

  return 0;
}

