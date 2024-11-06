#include <mm_manager.h>
#include <queue.h>

typedef struct queue* queueptr;

typedef struct queue {
    Node* first;
    Node* last;
    int size; //Ver si hace falta dejarlo o lo puedo sacar
} queue;

typedef struct Node{
    void* data;
    struct Node* next;
}Node;

queueptr create_queue(){
    queueptr q = mymalloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    q->size = 0;
    return q;
}

void enqueue(queueptr q, void* data){
    Node* new_node = mymalloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    if(q->size == 0){
        q->first = new_node;
        q->last = new_node;
    }else{
        q->last->next = new_node;
        q->last = new_node;
    }
    q->size++;
}

void* dequeueCertainData(queueptr q, void* data){
    Node * current = q->first;
    Node * previous = NULL;
    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        return NULL;
    }
    if (previous == NULL) {
        q->first = current->next;
    } else {
        previous->next = current->next;
    }
    if (current == q->last) {
        q->last = previous;
    }
    void * dataa = current->data;
    myfree(current);
    return dataa;
}