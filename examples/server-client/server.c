
#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    scmc_create(&self);

    self.usr_mem.integer = 123456;
    self.usr_mem.symbol = '#';

    // for hosting/server setup
    scmc_print_host(&self);


    // PROGRAM
    int is_running = 1;
    while (is_running) {

    };

    return 0;
}
