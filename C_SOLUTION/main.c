// LIBRARIES
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <pthread.h>



// CONSTANTS
// Let n be the number of philosophers
const int n = 3;


// MACROS
// Use this macro if shit hit the fan.
#define PANIC(MSG, ...) {                           \
    (void) fprintf (stderr, MSG "\n", __VA_ARGS__); \
    exit(EXIT_FAILURE);                             \
}

// Just a variadic free macro. Use it to free a lot of stuf at once.
#define FREE(X, ...) {                                                      \
    void* __to_be_freed[] = {X, __VA_ARGS__};                               \
    int __n_to_be_freed = sizeof(__to_be_freed) / sizeof(__to_be_freed[0]); \
    for (int __i = 0; i < __n_to_be_freed; i++)                             \
        free (__to_be_freed);                                               \
}


// GLOBAL VARIABLES
int* tids;
pthread_t* philosophers;
pthread_mutex_t* chopsticks;
pthread_mutex_t bowl;



// MAIN FUNCTION
int main (int argc, char *argv[]) {
    // ALLOCATING MEMORY
    philosophers = (pthread_t*) malloc (n * sizeof(pthread_t));
    if (NULL == philosophers) // ERROR HANDLING
        PANIC("FAILED TO ALLOCATE MEMORY FOR THE PHILOSOPHERS PTHREAD VECTOR.\n");

    chopsticks = (pthread_mutex_t*) malloc ((n + 1) * sizeof(pthread_mutex_t));
    if (NULL == chopsticks) { // ERROR HANDLING
        free(philosophers); 
        PANIC("FAILED TO ALLOCATE MEMORY FOR THE PHILOSOPHERS PTHREAD VECTOR.");
    }

    // INITIALIZING GLOBAL VARIABLES
    pthread_mutex_init ()


    // FREEING THE MEMORY
}