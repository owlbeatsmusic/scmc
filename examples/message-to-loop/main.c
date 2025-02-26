
/*

    In this example SCMC is used to send commands to a loop within 
    another process in a way that does not require the loop to 
    wait for an input (or in this case a command). 

    This works by checking this files UsrMem buffer each iteration 
    to detect any commands such as `PRINT_ONLY_I`. If found
    certain code can run (in this case change style of printing i
    by setting print_only_i to true/false).

*/

#include <stdio.h>
#include <stdbool.h>

// Imports for sleep
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#ifdef __unix__
#include <unistd.h>
#endif // __unix__

#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(void) {

    // Initialize SCMC process-information
    UsrInfo self;
    UsrInfo connection;
    scmc_create(&self);

    // Print PID and address (input for other process)
    scmc_print_host(&self);

    int max_iterations = 100;
    bool print_only_i = false;

    for (int i = 0; i < max_iterations; i++) {

        // Check if buffer is changed to command by other process
        if (strcmp(self.usr_mem.buffer, "PRINT_ONLY_I") == 0) {
            print_only_i = !print_only_i;
            strcpy(self.usr_mem.buffer, ""); // clear to not trigger multiple times
        }

        // Print in the style chosen by other process
        if (print_only_i  == true) {
            printf("%d\n", i);
        }
        else {
            printf("iteration: %d of %d\n", i, max_iterations);
        }

        // Wait 1 second
        #ifdef _WIN32
            Sleep(1000);
        #endif // _WIN32
        #ifdef __unix__
            sleep(1);
        #endif // __unix__
    }

    return 0;
}
