#include <mm_manager.h>
#include <process.h>
#include <queue.h>
#include <scheduler.h>
#include <stdbool.h>

#define MAX_SEMAPHORES       200
#define FIRST_USER_SEMAPHORE 100
#define TRUE 1
#define FALSE 0

typedef int sem_t;

typedef struct TSem {
	char * name;
	size_t value;
	queueptr blockedProcesses;
	int activeProcesses[MAX_PROCESSES];  // Keeps track of the processes that have permission to access the semaphore
	size_t activeProcessDim;
	int destroying; //Puede ser que no haga falta o lo cambio a int 
} TSem;

typedef struct TMutex {
	queueptr blockedProcesses;
	char value;
} TMutex;

void semInit();

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