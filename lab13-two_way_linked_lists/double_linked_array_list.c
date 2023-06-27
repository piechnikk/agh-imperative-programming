#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	size_t digits_count;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
	Node *prev = list->tail->prev;
    list->tail->prev = create_node(data, array_size, list->tail, list->tail->prev);
	prev->next = list->tail->prev;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
	Node *actual_node = list->head->next;
	while(n > actual_node->array_size){
		n-=actual_node->array_size;
		actual_node = actual_node->next;
	}
	return actual_node->data[n-1];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
	Node *actual_node = list->tail->prev;
	while(n > actual_node->array_size){
		n-=actual_node->array_size;
		actual_node = actual_node->prev;
	}
	return actual_node->data[actual_node->array_size-n];
}

void remove_node(Node *node_ptr) {
	Node *prev = node_ptr->prev;
	Node *next = node_ptr->next;
	prev->next = next;
	next->prev = prev;
	free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
	Node *actual_node = list->head->next;
	while(n > actual_node->array_size){
		n-=actual_node->array_size;
		actual_node = actual_node->next;
	}
	if(actual_node->array_size==1){
		remove_node(actual_node);
	}else{
		memmove((actual_node->data+n-1), (actual_node->data+n), (actual_node->array_size-n)*sizeof(int));
		actual_node->array_size--;
	}

}

// return the number of digits of number n
size_t digits(int n) {
	int count = 1;
	while(abs(n)>=10){
		n/=10;
		count++;
	}
	return count;
}

int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
	int digits_count = digits(value);

	Node *actual_node = list->head;
	while(actual_node->next != list->tail && actual_node->next->digits_count <= digits_count){
		actual_node = actual_node->next;
	}
	int *tab = (int*) safe_malloc(sizeof(int));
	*tab = value;
	if(actual_node->digits_count == digits_count){
		actual_node->array_size++;
		actual_node->data = realloc(actual_node->data, actual_node->array_size*sizeof(int));
		actual_node->data[actual_node->array_size-1] = value;
		qsort(actual_node->data, actual_node->array_size, sizeof(int), cmp);
	}else if(actual_node->next == list->tail){
		// push back
		Node *prev = list->tail->prev;
		list->tail->prev = create_node(tab, 1, list->tail, list->tail->prev);
		prev->next = list->tail->prev;
		prev->next->digits_count = digits_count;
	}else if(actual_node->next == list->head){
		// push front
		Node *next = list->head->next;
		list->head->next = create_node(tab, 1, list->head->next, list->head);
		next->prev = list->head->next;
		next->prev->digits_count = digits_count;
	}else{
		printf("mid=%i , ", value);
		Node *next = actual_node->next;
		actual_node->next = create_node(tab, 1, actual_node->next, actual_node);
		next->prev = actual_node->next;
		next->prev->digits_count = digits_count;
	}
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}