#ifndef SCMC_H_
#define SCMC_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mach/mach.h>


typedef struct {
    int integer;
    char symbol;
} UsrMem;

typedef struct {
    pid_t pid;
    UsrMem usr_mem;
    vm_address_t usr_mem_addr;

    kern_return_t res; // only for self_client
    mach_port_t task;
} UsrInfo;


/* CLIENT */

/* function from user: 'aaabbbccc' (https://stackoverflow.com/a/78169925)*/
void client_check_result(kern_return_t kern_res, char * msg) {
    if (kern_res != KERN_SUCCESS) {
            if (msg == NULL)
                    fprintf(stderr, "%s\n", mach_error_string(kern_res));
            else
                    fprintf(stderr, "%s : %s\n", msg, mach_error_string(kern_res));
            exit(1);
    }
}

void client_read_data(UsrInfo *self_client, UsrInfo *connection) {
    mach_msg_type_number_t size = sizeof(UsrMem);
    vm_size_t size_to_read = (vm_size_t) sizeof(UsrMem);
    void *data;

    self_client->res = vm_read(connection->task, connection->usr_mem_addr, size_to_read, (vm_offset_t *) &data, &size);
    client_check_result(self_client->res, "error reading virtual memory");
    memcpy(&connection->usr_mem, (UsrMem *)data, sizeof(UsrMem));
    fprintf(stdout, "%c\n", connection->usr_mem.symbol);
    printf("reading...\n");
}

void client_write_data(UsrInfo *self_client, UsrInfo *connection) {
    mach_msg_type_number_t size = sizeof(UsrInfo);
    connection->usr_mem.symbol = '!';
    self_client->res = vm_write(connection->task, connection->usr_mem_addr, (vm_offset_t) &connection->usr_mem, size);
    client_check_result(self_client->res, "error writing virtual memory");
}

void client_connect(UsrInfo *self_client, UsrInfo *connection) {
    self_client->res = task_for_pid(self_client->task, connection->pid, &connection->task);
    client_check_result(self_client->res, "error getting task");
}

void client_create(UsrInfo *self_client) {
    self_client->task = mach_task_self();
    self_client->pid = getpid();
}


/* HOST */

void host_memory(UsrInfo *self_client) {
    self_client->pid = getpid();
    printf("sudo ./client %d %p\n", self_client->pid, &self_client->usr_mem);
}




#endif
