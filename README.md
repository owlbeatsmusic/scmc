# SCMC
A simple library to connect C programs together via memory. Made primarily for learning purposes but can still be useful (although it's not the best alternative). Currently it **only works on MacOS** but is planned to expand to multiple platforms. 

It works by connecting processes via Process ID (PID) and then reading each others memory.

## Setup
To use the library just copy and paste the header file into your project (stb-style).

---

### Example of a simple "server" and "client" </br>
In this example the client reads and updates the UsrMem struct of the server. Although these files share the same header file, this is not necessary and will still work with two copies of the header file on different places of the system.

</br>

**server.c**:
```c
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
```
</br>

**client.c**:
```c
#include <stdlib.h>

#define SCMC_IMPLEMENTATION
#include "../../scmc.h"

int main(int argc, char *argv[]) {

    // SETUP
    UsrInfo self;
    UsrInfo connection;
    scmc_create(&self);


    connection.pid = atoi(argv[1]);
    connection.usr_mem_addr = strtoul(argv[2], NULL, 16);

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
```