#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
    void* data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector* vector, size_t block_size, size_t element_size) {
    (*vector).element_size = element_size;
    (*vector).data = malloc((*vector).element_size * block_size);
    (*vector).size = 0;
    (*vector).capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector* vector, size_t new_capacity) {
    if (new_capacity > (*vector).capacity) {
        (*vector).data = realloc((*vector).data, (*vector).element_size * new_capacity);
        (*vector).capacity = new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector* vector, size_t new_size) {
    reserve(vector, new_size);
    memset(((*vector).data + ((*vector).size * (*vector).element_size)), 0, (new_size - (*vector).size) * (*vector).element_size);
    (*vector).size = new_size;
}

// Add element to the end of the vector
void push_back(Vector* vector, void* value) {
    int prev_size = (*vector).size;
    int el_size = (*vector).element_size;
    resize(vector, prev_size + 1);
    memcpy(((*vector).data + (prev_size * el_size)), value, el_size);
}

// Remove all elements from the vector
void clear(Vector* vector) {
    free((*vector).data);
    (*vector).capacity = 0;
    (*vector).size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector* vector, size_t index, void* value) {
    int prev_size = (*vector).size;
    int el_size = (*vector).element_size;
    resize(vector, prev_size + 1);
    memmove(((*vector).data + ((index + 1) * el_size)), ((*vector).data + (index * el_size)), el_size * (prev_size - index));
    memcpy(((*vector).data + (index * el_size)), value, el_size);
}

// Erase element at position index
void erase(Vector* vector, size_t index) {
    int prev_size = (*vector).size;
    int el_size = (*vector).element_size;
    memmove(((*vector).data + (index * el_size)), ((*vector).data + ((index + 1) * el_size)), el_size * (prev_size - index - 1));
    (*vector).size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector* vector, void* value, cmp_ptr cmp) {
    for (int i = (*vector).size - 1; i >= 0; i--) {
        if (cmp(((*vector).data + (i * (*vector).element_size)), value) == 0) {
            erase(vector, i);
        }
    }

}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector* vector, int (*predicate)(void*)) {
    for (int i = (*vector).size - 1; i >= 0; i--) {
        if (predicate(((*vector).data + (i * (*vector).element_size)))) {
            erase(vector, i);
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector* vector) {
    (*vector).data = realloc((*vector).data, (*vector).element_size * (*vector).size);
    (*vector).capacity = (*vector).size;
}

// integer comparator
int int_cmp(const void* v1, const void* v2) {
    const int* cv1 = v1;
    const int* cv2 = v2;
    return *cv1 - *cv2;
}

// char comparator
int char_cmp(const void* v1, const void* v2) {
    const char* cv1 = v1;
    const char* cv2 = v2;
    return *cv1 - *cv2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void* p1, const void* p2) {
    const Person* cp1 = p1;
    const Person* cp2 = p2;
    int age_cmp = (*cp2).age - (*cp1).age;
    if (age_cmp) return age_cmp;
    int first_name_cmp = strcmp((*cp1).first_name, (*cp2).first_name);
    if (first_name_cmp) return first_name_cmp;
    return strcmp((*cp1).last_name, (*cp2).last_name);
}

// predicate: check if number is even
int is_even(void* value) {
    int val = *((int*)value);
    return val % 2 == 0;
}

// predicate: check if char is a vowel
int is_vowel(void* value) {
    char val = *((char*)value);
    return val == 'o' || val == 'O' || val == 'a' || val == 'A' || val == 'i' || val == 'I' || val == 'e' || val == 'E' || val == 'u' || val == 'U' || val == 'y' || val == 'Y';
}

// predicate: check if person is older than 25
int is_older_than_25(void* person) {
    Person* p = person;
    return (*p).age > 25;
}

// print integer value
void print_int(const void* v) {
    const int* val = v;
    printf("%i ", *val);
}

// print char value
void print_char(const void* v) {
    const char* val = v;
    printf("%c ", *val);
}

// print structure Person
void print_person(const void* v) {
    const Person* person = v;
    printf("%i %s %s\n", (*person).age, (*person).first_name, (*person).last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector* vector, print_ptr print) {
    printf("%i\n", (*vector).capacity);
    for (int i = 0; i < (*vector).size; i++) {
        print(((*vector).data + (i * (*vector).element_size)));
    }
}

// read int value
void read_int(void* value) {
    scanf("%i", (int*)value);
}

// read char value
void read_char(void* value) {
    scanf(" %c", (char*)value);
}

// read struct Person
void read_person(void* value) {
    scanf("%i %s %s", &(*(Person*)value).age, &(*(Person*)value).first_name, &(*(Person*)value).last_name);
}

void vector_test(Vector* vector, size_t block_size, size_t elem_size, int n, read_ptr read,
    cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
    init_vector(vector, block_size, elem_size);
    void* v = malloc(vector->element_size);
    size_t index, size;
    for (int i = 0; i < n; ++i) {
        char op;
        scanf(" %c", &op);
        switch (op) {
        case 'p': // push_back
            read(v);
            push_back(vector, v);
            break;
        case 'i': // insert
            scanf("%zu", &index);
            read(v);
            insert(vector, index, v);
            break;
        case 'e': // erase
            scanf("%zu", &index);
            erase(vector, index);
            break;
        case 'v': // erase
            read(v);
            erase_value(vector, v, cmp);
            break;
        case 'd': // erase (predicate)
            erase_if(vector, predicate);
            break;
        case 'r': // resize
            scanf("%zu", &size);
            resize(vector, size);
            break;
        case 'c': // clear
            clear(vector);
            break;
        case 'f': // shrink
            shrink_to_fit(vector);
            break;
        case 's': // sort
            qsort(vector->data, vector->size,
                vector->element_size, cmp);
            break;
        default:
            printf("No such operation: %c\n", op);
            break;
        }
    }
    print_vector(vector, print);
    free(vector->data);
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
    case 1:
        vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
            is_even, print_int);
        break;
    case 2:
        vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
            is_vowel, print_char);
        break;
    case 3:
        vector_test(&vector_person, 2, sizeof(Person), n, read_person,
            person_cmp, is_older_than_25, print_person);
        break;
    default:
        printf("Nothing to do for %d\n", to_do);
        break;
    }

    return 0;
}