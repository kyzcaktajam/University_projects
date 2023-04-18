#include <stdlib.h>
#include <stdbool.h>

struct Node {
    unsigned long long int value;
    unsigned long long int distance;
    struct Node *next;
};

typedef struct Node Node;
static Node *head = NULL, *tail = NULL;

bool queue_insert(unsigned long long int x, unsigned long long int d) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
        return false;
        
    newNode->value = x;
    newNode->next = NULL;
    newNode->distance = d;
    
    if(head == NULL && tail == NULL)
        head = tail = newNode;
    else {
        tail->next = newNode;
        tail = newNode;
    }
    return true;
}

bool queue_is_empty(void) {
    if (head == NULL) 
        return true;
    else return false;
}

unsigned long long int queue_front_value(void) {
    return head->value;
}

unsigned long long int queue_front_distance(void) {
    return head->distance;
}

void queue_pop(void) {
	if (head == tail) {
		free(head);
		head = NULL;
		tail = NULL;
		return;
	}
    Node *front = head;
    head = head->next;
    free(front);
}

void free_queue(void) {
	while (!queue_is_empty()) {
		queue_pop();
	}
}
