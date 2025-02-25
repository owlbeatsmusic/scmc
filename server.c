#include "scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    client_create(&self);

    self.usr_mem.integer = 123456;
    self.usr_mem.symbol = '#';

    // for hosting/server setup
    host_memory(&self);


    // PROGRAM
    int is_running = 1;
    while (is_running) {

    };

    return 0;
}
