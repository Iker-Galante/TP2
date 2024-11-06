#include <stdint.h>
#include <stddef.h>
#include <mm_manager.h>
#include <queue.h>
#include <process.h>

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