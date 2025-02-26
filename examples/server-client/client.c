#include <stdlib.h>

#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    scmc_create(&self);

    connection.pid = atoi(argv[1]);
    connection.usr_mem_addr = (LPCVOID)(uintptr_t)strtoul(argv[2], NULL, 16); // Use uintptr_t for conversion

    // for client setup
    scmc_connect(&self, &connection);

    // PROGRAM
    scmc_read_data(&self, &connection);
    printf("usrmem=(%d, '%c')\n", connection.usr_mem.integer, connection.usr_mem.symbol);

    printf("2");
    connection.usr_mem.symbol = '!';
    connection.usr_mem.integer += 1;
    scmc_write_data(&self, &connection);

    scmc_read_data(&self, &connection);
    printf("usrmem=(%d, '%c')\n", connection.usr_mem.integer, connection.usr_mem.symbol);

    scanf(":");

    return 0;
}