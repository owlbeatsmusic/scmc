#ifndef SCMC_H_
#define SCMC_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef __APPLE__
    #include <unistd.h>
    #include <mach/mach.h>
#endif

#ifdef _WIN32
    #include <windows.h>
    #include <memoryapi.h>
    #include <processthreadsapi.h>
    #include <WinUser.h>
#endif // _WIN32


/* The data that other processes can access. */
typedef struct {
    int integer;
    char symbol;
} UsrMem;

/* Information abut a process and UsrMem (+ its address). */
typedef struct {

    UsrMem usr_mem;

    #ifdef _WIN32
        DWORD pid;
        HANDLE handle;
        LPCVOID usr_mem_addr;
    #endif // _WIN32

    #ifdef __APPLE__
        pid_t pid;
        mach_port_t task;
        vm_address_t usr_mem_addr;

        kern_return_t res; // only for client
    #endif // __APPLE__

} UsrInfo;

void scmc_read_data(UsrInfo *self, UsrInfo *connection);
void scmc_write_data(UsrInfo *self, UsrInfo *connection);
void scmc_connect(UsrInfo *self, UsrInfo *connection);
void scmc_create(UsrInfo *self);

void scmc_print_host(UsrInfo *self);

#endif // SCMC_H_


#ifdef SCMC_IMPLEMENTATION

#ifdef __APPLE__
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
#endif // __APPLE__

/* Reads the changes of UsrMem in the memory of the connected process and updates `connection->usr_mem`. */
void scmc_read_data(UsrInfo *self, UsrInfo *connection) {
    unsigned int size = sizeof(UsrMem);
    unsigned int size_to_read = sizeof(UsrMem);
    void *data;

    #ifdef __APPLE__
        self->res = vm_read(connection->task, connection->usr_mem_addr, (vm_size_t)size_to_read, (vm_offset_t *) &data, (mach_msg_type_number_t *)&size);
        scmc_check_result(self->res, "error reading virtual memory");
    #endif // __APPLE__

    #ifdef _WIN32
        int read_successful = ReadProcessMemory(connection->handle, connection->usr_mem_addr, (LPVOID)data, (SIZE_T)size, (SIZE_T *)&size_to_read);
        if (read_successful == 0) {
            printf("Error reading virtual memory: %d\n", GetLastError());
        }
    #endif // _WIN32

    memcpy(&connection->usr_mem, (UsrMem *)data, sizeof(UsrMem));
}

/* Writes changes of `connection->usr_mem` to the actual UsrMem of the connected process. */
void scmc_write_data(UsrInfo *self, UsrInfo *connection) {
    unsigned int size = sizeof(UsrMem);
    unsigned int size_to_read = sizeof(UsrMem);
    #ifdef __APPLE__
        self->res = vm_write(connection->task, connection->usr_mem_addr, (vm_offset_t) &connection->usr_mem, (mach_msg_type_number_t)size);
        scmc_check_result(self->res, "error writing virtual memory");
    #endif // __APPLE__

    #ifdef _WIN32
        int write_successful = WriteProcessMemory(connection->handle, (LPVOID)connection->usr_mem_addr, (LPCVOID)&connection->usr_mem, (SIZE_T)size, (SIZE_T *)&size_to_read);
        if (write_successful == 0) {
            printf("Error reading virtual memory: %d\n", GetLastError());

        }
    #endif // _WIN32
}

/* Establish the connection between the two processes and set the 'connection->usr_info' (PID, ...). */
void scmc_connect(UsrInfo *self, UsrInfo *connection) {
    #ifdef __APPLE__
        self->res = task_for_pid(self->task, connection->pid, &connection->task);
        scmc_check_result(self->res, "error getting task");
    #endif // __APPLE__

    #ifdef _WIN32
        connection->handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, TRUE, connection->pid);
        if (connection->handle == NULL) {
            printf("scmc_connect: Error reading virtual memory: %d\n", GetLastError());
        }
    #endif // _WIN32
}

/* Set UsrInfo (PID, ...) of the process from where this function is called from. */
void scmc_create(UsrInfo *self) {
    #ifdef __APPLE__
        self->task = mach_task_self();
        self->pid = getpid();
    #endif // __APPLE__

    #ifdef _WIN32
        self->pid = GetCurrentProcessId();
        //self->handle = OpenProcess(PROCESS_ALL_ACCESS , TRUE, GetCurrentProcessId());
        //if (self->handle == NULL) {
        //    printf("scmc_create: Error reading virtual memory: %d\n", GetLastError());
        //}
    #endif // _WIN32
}


/* Prints the PID and address(of UsrMem) formatted: `"[scmc]: {PID} {ADDRESS}"`.*/
void scmc_print_host(UsrInfo *self) {
    printf("[scmc]: %d %p\n", self->pid, &self->usr_mem);
}


#endif // SCMC_IMPLEMENTATION