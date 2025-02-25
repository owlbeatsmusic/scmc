# SCMC
A simple library to connect C programs together via memory. Made primarily for learning purposes but can still be useful (although it's not the best alternative). Currently it **only works on MacOS** but is planned to expand to multiple platforms. 

It works by connecting processes via Process ID (PID) and then reading each others memory.

---

### Example of a simple "server" and "client" </br>
**server.c**:
```c
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
```
</br>

**client.c**:
```c
#include <stdlib.h>

#include "scmc.h"

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
```