# SCMC
A simple library to connect C programs together via memory. Made primarily for learning purposes but can still be useful (although it's not the best alternative). 

> [!NOTE]
> Works on **Windows, Macos** (not yet on Linux)

It works by connecting processes via Process ID (PID) and then reading each others memory.

</br>

### Setup
To use the library just copy and paste the header file into your project (stb-style). Two processes does not need to share the same scmc.h file to be able to connect. The processes can be on two different locations of the system. 

</br>

### Example of a simple "server" and "client" </br>
(`/examples/server-client/`) 

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
    self.usr_mem.symbol = 'a';

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
```