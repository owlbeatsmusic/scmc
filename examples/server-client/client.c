
/*

    In this example SCMC is used to emulate the usage of a server
    and client (in a very basic form). This example is also 
    pretty much the most bare minimum way to use the library in
    all cases (SCMC works as sender/receiver ≈ client/server).

    This example works by first creating the server by simply 
    getting and setting the PID (and depending on OS other 
    information about the process) and then printing the PID and 
    address of UsrMem.
    
    The client takes the servers printed PID and address as 
    command line arguments then also gets and sets process information. 
    Then a connection between the two processes is created. Now 
    everything is set up for reading/writing. In this example the 
    servers UsrMem is read and then modified by incrementing the first 
    integer and changing the first char then writing the changes to the
    servers memory and lastly reading the chnages to verify.

*/

#include <stdlib.h>

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
    printf("usrmem=(%d, '%c')\n", connection.usr_mem.integer, connection.usr_mem.symbol);

    connection.usr_mem.symbol = '!';
    connection.usr_mem.integer += 1;
    scmc_write_data(&self, &connection);

    scmc_read_data(&self, &connection);
    printf("usrmem=(%d, '%c')\n", connection.usr_mem.integer, connection.usr_mem.symbol);

    return 0;
}
