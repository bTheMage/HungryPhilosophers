/**
 * @file main.c
 * @author Bernardo Maia Coelho (bernardomc@usp.br) (12542481)
 * @brief An implementation of the Dining Philosophers Problem.
 * @date 2022-11-06
 * 
 */



// LIBRARIES
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>



// CONSTANTS
// Let n be the number of dining philosophers
// Although you can set N to be any positive non zero integer, big numbers will
// make it hard to display the philosophers state.
#define N (5)

// A second in microsseconds
#define SECOND (1000000)

// The period between each print in microsseconds.
#define DISPLAY_PERIOD (1 + SECOND / 3)

// The time each philosopher speends thinking and eating (idealy) in microsseconds.
#define THINKING_PERIOD (1 * SECOND)
#define EATING_PERIOD (1 * SECOND)



// MACROS
// Use this to stop execution.
#define PANIC(MSG) {fprintf (stderr, MSG); exit(EXIT_FAILURE);}                                                           

// OBS: "phi" will be used as an abbreviation for "philosopher" from this point on
// The following macros will return the index (ids) for the philosophers on the
// left and right of a given philosopher passed to the macro as an id (index).
#define LEFT_PHI(PHI_ID) ((PHI_ID + 1) % N)
#define RIGHT_PHI(PHI_ID) ((PHI_ID + (N - 1)) % N)



// TYPES
// Let THINKING, HUNGRY and EATING be the 3 possible states for a philosopher
typedef enum {THINKING = 0, HUNGRY = 1, EATING = 2} PHI_STATE;



// GLOBAL VARIABLES
// A vector of the philosophers states.
PHI_STATE phi_states [N];

// A vector os mutexes and conditional variables to emulate monitors.
pthread_mutex_t phi_mutex[N];
pthread_cond_t phi_cond [N];

// A vector of thread ids whick will emulate the philosophers trying to access
// shared resources (chopsticks).
pthread_t phi_threads[N];



// FUNCTIONS DECLARATION
void* runner (void* param);
void init ();
void pickup (int phi_id);
void test (int phi_id);
void putdown (int phi_id);



// MAIN FUNCTION
int main (int argc, char *argv[]) {
    // LOCAL VARIABLES AND CONSTANTS
    // This variable serves to convert and display the state of a philosopher.
    const char phi_states_names[3][10] = 
        #if (N <= 7)
            {"THINKING ", "HUNGRY   ", "EATING   "}
        #else
            {"T ", "H ", "E "}
        #endif
    ;

    // This variable is just a clock that counts how many times we display 
    // something on the terminal.
    int clk = 0;
    printf("%d) \t", clk++);


    // CREATING THREADS
    for (int i = 0; i < N; i++) {
        // Creating the thread
        if ( pthread_create (&phi_threads[i], NULL, runner, (void*)((intptr_t)i)) )
            PANIC("Failed to create a thread!");

        // Just displaying the philosopher id.
        #if (N <= 7)
            printf("%d       \t", i);
        #else
            printf("%d \t", i);
        #endif
    }
    printf("\n");

    // PRINTING THE PHILOSOPHERS STATE
    // 4 times per second
    while (true)
    {
        // Printing the clock
        printf("%d) \t", clk++);

        // Printing the state of each pylosopher
        for (int i = 0; i < N; i++)
            printf("%s\t", phi_states_names[phi_states[i]]);
        
        printf("\n");

        // Sleeping for 1/4 seconds
        usleep(DISPLAY_PERIOD); // You can change the exact time.
    }

    // JOINING THREADS
    // From this point foward, no line will be reached since we have a "while(true)"
    // endless loop above. So, those lines of code serve no purpose other then to 
    // show how it could be coded on similar programs.
    for (int i = 0; i < N; i++) {
        pthread_join (phi_threads[i], NULL);
        pthread_cond_destroy (&phi_cond[i]);
        pthread_mutex_destroy (&phi_mutex[i]);
    }

    // FINILIZING THE EXECUTION
    printf("All done!\n");
}


// Just a function to initiate the variables
void init () {
    for (int i = 0; i < N; i++) {
        // At the beginning, all philosophers are thinking
        phi_states[i] = THINKING;

        // We need to initilize the conditional variable and its mutex
        pthread_mutex_init(&phi_mutex[i], NULL);
        pthread_cond_init(&phi_cond[i], NULL);
    }
}

// A philosopher's though: Lets pickup a chop stick!
void pickup (int phi_id) {
    // I AM HUNGRY!!!
    phi_states[phi_id] = HUNGRY;

    // CAN I EAT NOW!?
    test(phi_id);

    // WAITING TO EAT
    if (phi_states[phi_id] != EATING) {
        // Obs: Since C does not provide a monitor, we use a condition variable
        // with a POSIX mutex lock.
        pthread_mutex_lock(&phi_mutex[phi_id]);
        while (phi_states[phi_id] != EATING)
            pthread_cond_wait(&phi_cond[phi_id], &phi_mutex[phi_id]);
        pthread_mutex_unlock(&phi_mutex[phi_id]);
    }
}

// Testing if i can eat
void test(int phi_id)
{
    bool right_phi_isnt_eating = (phi_states[RIGHT_PHI(phi_id)] != EATING);
    bool left_phi_isnt_eating = (phi_states[LEFT_PHI(phi_id)] != EATING);
    bool we_are_hungry = (phi_states[phi_id] == HUNGRY);

    bool we_should_eat_now = left_phi_isnt_eating && right_phi_isnt_eating &&
        we_are_hungry;

    if (we_should_eat_now) {
        // indicate that I’m eating
        phi_states[phi_id] = EATING;

        // signal() has no effect during Pickup(),
        // but is important to wake up waiting
        // hungry philosophers during Putdown()
        pthread_cond_signal(&phi_cond[phi_id]);
    }
}

// Lets putdown the chopsticks
void putdown(int phi_id)
{
    // I'm full, so back to thinking!
    phi_states[phi_id] = THINKING;

    // Lets wake our neighbor
    test(RIGHT_PHI(phi_id));
    test(LEFT_PHI(phi_id));
}

// This function will be executed by each thread.
void* runner (void* param) {
    // Getting my id
    int my_id = (int)((intptr_t)param);

    // Forever!
    while (true) {
        // lets think for a second
        usleep(THINKING_PERIOD);  // You can change the time if you want.

        // Well, now i'm hungry! So i will pickup some chopsticks.
        pickup(my_id); 

        // lets eat for a second
        usleep(EATING_PERIOD); // You can change the time if you want.

        // Not hungry anymore, so i will putdown the chopsticks.
        putdown(my_id);
    }
}