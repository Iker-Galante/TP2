#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define READY 0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE 3
#define DEAD 4

#define READ_FD 0
#define WRITE_FD 1
// SHELL ES EL 0

#define MAX_PROCESSES 100 // Maximo de procesos que se pueden crear un array de PCBs

typedef struct memoryBlock{
    uint64_t *base;
    size_t size;
    uint64_t *current;
} memoryBlock;

typedef struct Process_Control_Block {
    size_t priority;
    size_t ticks;
    int pid;
    char *name;
    int parent;
    memoryBlock *stack;
    int status;
    int fd[2]; // 0 es lectura, 1 es escritura (por ahora solo 1)
    char foreground;
    int semId; // -1 si no tiene semaforo
    int exitCode; // -1 si no termino
} PCB;

typedef struct processInfo{
    char * name;
	int pid;
	int parent;
	uint64_t * rsp;
	uint64_t * rbp;
	size_t priority;
	char foreground;
	int status;
	int exitCode;
}processInfo;

int createProcess(char * name, int parent, size_t stackSize, char ** args, void * function, char foreground, int * fds);

int killProcess(int pid);

int killChildren(int parentPid);

int blockProcess(int pid);

int unblockProcess(int pid);

void setFileDescriptor(int pid, int index, int value);

processInfo ** getProcessesInfo();

void freeProcessesInfo(processInfo ** info);

void freeProcess(PCB * processPcb);

int waitpid(int pid);

void resetProcesses();

void processWrapper(int function(char ** args), char ** args);

#endif