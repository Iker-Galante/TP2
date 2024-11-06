#include <mm_manager.h>

typedef struct queue* queueptr;

queueptr create_queue();
void enqueue(queueptr q, void* data);
void* dequeueCertainData(queueptr q, void* data);