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
#define PANIC(MSG) {fprintf (stderr, MSG); exit(EXIT_FAILURE);}

// This macro iterates throgh a fixed size static array and free all its pointers.
#define FREEEACH(ARRAY) {                                   \
    int __num_of_pnt = sizeof(ARRAY) / sizeof(ARRAY[0]);    \
    for (int __i = 0; __i < __num_of_pnt; __i++)            \
        free (ARRAY[__i]);                                  \
}                                                           

// GLOBAL VARIABLES
int* tids;
int* chopsticks_state;
pthread_t* philosophers;
pthread_mutex_t* chopsticks;
pthread_mutex_t bowl;



// MAIN FUNCTION
int main (int argc, char *argv[]) {
    printf("Hello, World!\n");

    // VARIABLES
    int mainflag = 0;

    // ALLOCATING MEMORY
    philosophers = (pthread_t*) malloc (n * sizeof(pthread_t));
    tids = (int*) calloc (n, sizeof(int));
    chopsticks_state = (int*) malloc ((n) * sizeof(int));
    chopsticks = (pthread_mutex_t*) malloc ((n + 1) * sizeof(pthread_mutex_t));

    void* pointers[] = {philosophers, tids, chopsticks_state, chopsticks};

    // ERROR HANDLING
    if (!(philosophers && tids && chopsticks_state && chopsticks)) {
        FREEEACH(pointers);
        PANIC("FAILED TO ALLOCATE MEMORY.");
    }

    // INITIALIZING GLOBAL VARIABLES
    for (int i = 0; i <= n; i++)
        if (pthread_mutex_init (&chopsticks[i], NULL)) {
            FREEEACH(pointers);
            PANIC("FAILED TO INITIATE A MUTEX.");
        }

    



    // DESTROYING THE MUTEXES
    for (int i = 0; i <= n; i++)
        if (pthread_mutex_destroy (&chopsticks[i])) {
            FREEEACH(pointers);
            PANIC("FAILED TO INITIATE A MUTEX.");
        }

    // FREEING THE MEMORY
    FREEEACH(pointers);


    printf("Good Bye, World!\n");
}