
/*

    See top of `main.c` in same folder for explanation of
    example.

*/

#include <stdlib.h>
#include <stdbool.h>

#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    scmc_create(&self);

    connection.pid = atoi(argv[1]);
    #ifdef _WIN32
        connection.usr_mem_addr = (LPCVOID)(uintptr_t)strtoul(argv[2], NULL, 16);
    #endif // _WIN32
    #ifdef __APPLE__
        connection.usr_mem_addr = (vm_adress_t)strtoul(argv[2], NULL, 16);
    #endif // __APPLE__

    // for client setup
    scmc_connect(&self, &connection);


    // PROGRAM
    scmc_read_data(&self, &connection);

    while (true) {
        char input_buffer[256];
        printf("input command (EXIT, PRINT_ONLY_I): ");
        scanf("%s", input_buffer);

        if (strcmp(input_buffer, "EXIT") == 0) break;

        strcpy(connection.usr_mem.buffer, input_buffer);
        scmc_write_data(&self, &connection);
    }
    
    return 0;
}
