#include <stdio.h>    // For fprintf, stderr, printf
#include <stdlib.h>   // For malloc, free, realloc
#include <stdbool.h>  // For bool type

// Core Structs:

typedef struct {
    void **collection;
    int capacity;
    int size;
    void (*print_fn)(const void *);
    void (*free_fn)(void *);
} Stack;

// Forward declaration for optional internal helper
static bool resize_stack(Stack *stack, int new_capacity);

//function prototypes
Stack *create_stack(int initial_capacity, void (*print_fn)(const void *), void (*free_fn)(void *));
void print_stack(const Stack *stack);
void destroy_stack(Stack *stack);

int main(){
    void (*print_fn)(const void *) = (void (*)(const void *))&print_stack;
    void (*free_fn)(void *) = (void (*)(void *))&destroy_stack; 

    create_stack(4, print_fn, free_fn);
    return 0;
}

// Functions:

Stack *create_stack(int initial_capacity, void (*print_fn)(const void *), void (*free_fn)(void *)) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Stack Creation Error: Initial capacity must be positive.\n");
        return NULL;
    }

    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (!stack) {
        fprintf(stderr, "Stack Creation Error: Failed to allocate memory for Stack struct.\n");
        return NULL;
    }

    stack->collection = (void **)malloc(sizeof(void *) * initial_capacity);
    if (!stack->collection) {
        fprintf(stderr, "Stack Creation Error: Failed to allocate memory for stack collection array.\n");
        free(stack);
        return NULL;
    }

    stack->capacity = initial_capacity;
    stack->size = 0;
    stack->print_fn = print_fn;
    stack->free_fn = free_fn;

    puts("stack creation successful\n");
    return stack;
}

void destroy_stack(Stack *stack) {
    if (!stack) {
        fprintf(stderr, "Stack Destruction Error: Cannot destroy a NULL stack.\n");
        return;
    }

    if (stack->collection) {
        if (stack->free_fn) {
            for (int i = 0; i < stack->size; i++) {
                stack->free_fn(stack->collection[i]);
            }
        }
        free(stack->collection);
    }
    free(stack);
}

// Optional internal helper: Resizes the stack's underlying array
static bool resize_stack(Stack *stack, int new_capacity) {
    if (!stack) {
        fprintf(stderr, "Resize Stack Error: Stack is NULL.\n");
        return false;
    }
    if (new_capacity <= stack->size) { // New capacity must be at least current size
        fprintf(stderr, "Resize Stack Error: New capacity (%d) must be greater than current size (%d).\n", new_capacity, stack->size);
        return false;
    }

    void **new_collection = (void **)realloc(stack->collection, sizeof(void *) * new_capacity);
    if (!new_collection) {
        fprintf(stderr, "Resize Stack Error: Failed to reallocate memory for stack collection.\n");
        return false;
    }
    stack->collection = new_collection;
    stack->capacity = new_capacity;
    return true;
}


bool is_full(const Stack *stack) {
    if (!stack) {
        fprintf(stderr, "Is Full Error: Stack is NULL.\n");
        return false; // A NULL stack can't be full in a meaningful way
    }
    return stack->size == stack->capacity;
}

bool is_empty(const Stack *stack) {
    if (!stack) {
        fprintf(stderr, "Is Empty Error: Stack is NULL.\n");
        return true; // A NULL stack is effectively empty
    }
    return stack->size == 0;
}

bool push(Stack *stack, void *item) {
    if (!stack) {
        fprintf(stderr, "Push Error: Stack is NULL.\n");
        return false;
    }
    if (!item) {
        fprintf(stderr, "Push Error: Cannot push NULL item.\n");
        return false;
    }

    if (is_full(stack)) { // Checks if stack->size == stack->capacity
        if (!resize_stack(stack, stack->capacity * 2)) {
            fprintf(stderr, "Push Error: Failed to resize stack. Push operation failed.\n");
            return false;
        }
    }

    stack->collection[stack->size] = item;
    stack->size++;
    return true;
}

bool pop(Stack *stack, void **popped_item_out) {
    if (!stack) {
        fprintf(stderr, "Pop Error: Stack is NULL.\n");
        return false;
    }
    if (is_empty(stack)) {
        fprintf(stderr, "Pop Error: Stack is empty. Nothing to pop.\n");
        return false;
    }
    if (!popped_item_out) {
        fprintf(stderr, "Pop Error: popped_item_out pointer cannot be NULL.\n");
        return false;
    }

    stack->size--; // Decrement size first, then access
    *popped_item_out = stack->collection[stack->size];
    // Optional: Shrink array if capacity is much larger than size
    // if (stack->size > 0 && stack->size < stack->capacity / 4) {
    //     resize_stack(stack, stack->capacity / 2);
    // }
    return true;
}

bool peek(const Stack *stack, void **peeked_item_out) {
    if (!stack) {
        fprintf(stderr, "Peek Error: Stack is NULL.\n");
        return false;
    }
    if (is_empty(stack)) {
        fprintf(stderr, "Peek Error: Stack is empty. Nothing to peek.\n");
        return false;
    }
    if (!peeked_item_out) {
        fprintf(stderr, "Peek Error: peeked_item_out pointer cannot be NULL.\n");
        return false;
    }

    *peeked_item_out = stack->collection[stack->size - 1];
    return true;
}

int len(const Stack *stack) {
    if (!stack) {
        fprintf(stderr, "Len Error: Stack is NULL.\n");
        return -1; // Sentinel for error
    }
    return stack->size;
}

void print_stack(const Stack *stack) {
    if (!stack) {
        fprintf(stderr, "Print Stack Error: Stack is NULL.\n");
        return;
    }
    if (!stack->print_fn) {
        fprintf(stderr, "Print Stack Warning: print_fn not provided. Cannot print stack elements.\n");
        return;
    }

    printf("Stack (Size: %d, Capacity: %d) - TOP -> [", stack->size, stack->capacity);
    for (int i = stack->size - 1; i >= 0; i--) { // Print from top to bottom
        stack->print_fn(stack->collection[i]);
        if (i > 0) {
            printf(" | ");
        }
    }
    printf("] <- BOTTOM\n");
}