#ifndef SCMC_H_
#define SCMC_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mach/mach.h>


/* The data that other processes can access. */
typedef struct {
    int integer;
    char symbol;
} UsrMem;

/* Information abut a process and UsrMem (+ its address). */
typedef struct {
    pid_t pid;
    UsrMem usr_mem;
    vm_address_t usr_mem_addr;

    kern_return_t res; // only for client
    mach_port_t task;
} UsrInfo;

static void scmc_check_result(kern_return_t kern_res, char * msg); /* Check for kernal rescheduling interrupt-errors. */
void scmc_read_data(UsrInfo *self, UsrInfo *connection);
void scmc_write_data(UsrInfo *self, UsrInfo *connection);
void scmc_connect(UsrInfo *self, UsrInfo *connection);
void scmc_create(UsrInfo *self);

void scmc_print_host(UsrInfo *self);

#endif // SCMC_H_


#ifdef SCMC_IMPLEMENTATION

/* function from user: 'aaabbbccc' (https://stackoverflow.com/a/78169925)*/
static void scmc_check_result(kern_return_t kern_res, char * msg) {
    if (kern_res != KERN_SUCCESS) {
            if (msg == NULL)
                    fprintf(stderr, "%s\n", mach_error_string(kern_res));
            else
                    fprintf(stderr, "%s : %s\n", msg, mach_error_string(kern_res));
            exit(1);
    }
}

/* Reads the changes of UsrMem in the memory of the connected process and updates `connection->usr_mem`. */
void scmc_read_data(UsrInfo *self, UsrInfo *connection) {
    mach_msg_type_number_t size = sizeof(UsrMem);
    vm_size_t size_to_read = (vm_size_t) sizeof(UsrMem);
    void *data;

    self->res = vm_read(connection->task, connection->usr_mem_addr, size_to_read, (vm_offset_t *) &data, &size);
    scmc_check_result(self->res, "error reading virtual memory");
    memcpy(&connection->usr_mem, (UsrMem *)data, sizeof(UsrMem));
}

/* Writes changes of `connection->usr_mem` to the actual UsrMem of the connected process. */
void scmc_write_data(UsrInfo *self, UsrInfo *connection) {
    mach_msg_type_number_t size = sizeof(UsrInfo);
    self->res = vm_write(connection->task, connection->usr_mem_addr, (vm_offset_t) &connection->usr_mem, size);
    scmc_check_result(self->res, "error writing virtual memory");
}

/* Establish the connection between the two processes and set the 'connection->usr_info' (task, PID). */
void scmc_connect(UsrInfo *self, UsrInfo *connection) {
    self->res = task_for_pid(self->task, connection->pid, &connection->task);
    scmc_check_result(self->res, "error getting task");
}

/* Set UsrInfo (task, PID) of the process from where this function is called from. */
void scmc_create(UsrInfo *self) {
    self->task = mach_task_self();
    self->pid = getpid();
}


/* Prints the PID and address(of UsrMem) formatted: `"[scmc]: {PID} {ADDRESS}"`.*/
void scmc_print_host(UsrInfo *self) {
    printf("[scmc]: %d %p\n", self->pid, &self->usr_mem);
}


#endif // SCMC_IMPLEMENTATION