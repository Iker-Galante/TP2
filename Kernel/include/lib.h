#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

/* Used for sound */
void outb(int memoryMap, int output); 
int inb(int memoryMap);

/*Used for Stack creation*/
uint64_t *createStack(uint64_t *stackStart, void *function, char **args);
void forceScheduler();
void forceProcessChange();

#endif