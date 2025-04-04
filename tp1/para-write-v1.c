#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Variable partagée entre les threads
uint32_t shared_counter = 0;
uint32_t nb_incr_per_thread = 0;

// Mutex pour assurer l'exclusion mutuelle lors de l'accès à shared_counter
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * f(void * arg) {
    (void) arg;
    for (uint32_t i = 0; i < nb_incr_per_thread; ++i) {
        // On verrouille le mutex avant d'incrémenter la variable partagée
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
        shared_counter += 1;
        // On déverrouille le mutex après l'incrémentation
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("pthread_mutex_unlock");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <nb-threads> <nb-incr>\n", argv[0]);
        return EXIT_FAILURE;
    }

    uint32_t nb_threads = (uint32_t)atoi(argv[1]);
    uint32_t nb_incr = (uint32_t)atoi(argv[2]);

    // Vérifier que le nombre de threads est positif et que nb_incr est divisible par nb_threads
    assert(nb_threads > 0);
    assert(nb_incr % nb_threads == 0);
    nb_incr_per_thread = nb_incr / nb_threads;

    printf("%u threads, chaque thread fait %u incrémentations\n", nb_threads, nb_incr_per_thread);
    printf("Valeur attendue : %u\n", nb_incr);

    // Allocation dynamique du tableau de threads
    pthread_t *threads = malloc(nb_threads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Création des threads
    for (uint32_t i = 0; i < nb_threads; ++i) {
        if (pthread_create(&threads[i], NULL, f, NULL) != 0) {
            perror("pthread_create");
            free(threads);
            exit(EXIT_FAILURE);
        }
    }

    // Attente de la fin de tous les threads
    for (uint32_t i = 0; i < nb_threads; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(threads);
            exit(EXIT_FAILURE);
        }
    }

    printf("Valeur calculée : %u\n", shared_counter);

    // Libération des ressources
    free(threads);
    if (pthread_mutex_destroy(&mutex) != 0) {
        perror("pthread_mutex_destroy");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
