#include <stdlib.h>

#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    client_create(&self);

    connection.pid = atoi(argv[1]);
    connection.usr_mem_addr = (vm_address_t)strtoul(argv[2], NULL, 16);
    
    // for client setup
    client_connect(&self, &connection);


    // PROGRAM
    client_read_data(&self, &connection);
    client_write_data(&self, &connection);
    client_read_data(&self, &connection);

    return 0;
}