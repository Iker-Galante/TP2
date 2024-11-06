#include <stdint.h>
#include <process.h>
#include <queue.h>
#include <scheduler.h>
#include <mm_manager.h>
#include <sem.h>

sem_t semCreate(char * name, int initValue);

int semDestroy(sem_t id);

int userExists(char * name); // Returns the index of the user in the activeProcesses array

sem_t semOpen(char * semName);

sem_t semClose(sem_t sem);

sem_t semCreateAnon(int initVal); // Creates an anonymous semaphore

sem_t semAnonOpen(sem_t sem); // Opens an anonymous semaphore

sem_t findMinFreeSemID(); // Finds the minimum free semaphore ID

int semWait(sem_t sem);

int semPost(sem_t sem);

int semSet(int semId, int value);

extern int criticalRegion(char * semValue);

// MUTEX

void initMutex();

void waitMutex(int id);

void postMutex(int id);

void createMutex(int id);

void delteMutex(int id);