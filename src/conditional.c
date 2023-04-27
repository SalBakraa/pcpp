#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

#include <stdbool.h>

typedef struct {
	bool conditional_is_undetermined;
	bool conditional_was_resolved;
	bool should_process;
	bool should_output;
} scope_item;


scope_item *scope_item_make() {
	scope_item *item = malloc(sizeof *item);
	if (item == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	memset(item, 0, sizeof *item);
	return item;
}

// Define the symbol table structure
typedef struct {
	scope_item **scopes;
	size_t count;
	size_t capacity;
} scope_stack;

scope_stack *scope_stack_make() {
	scope_stack *stack = malloc(sizeof *stack);
	if (stack == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	memset(stack, 0, sizeof *stack);
	stack->capacity = 10;
	stack->scopes = malloc(sizeof *stack->scopes * stack->capacity);
	if (stack->scopes == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	// The scope stack must have a global scope
	scope_item *item = scope_item_make();
	item->should_process = true;
	item->should_output = true;
	stack->scopes[stack->count++] = item;
	stack->capacity--;
	return stack;
}

scope_item *scope_stack_peek(scope_stack *stack) {
	if (stack->count == 0) {
		return NULL;
	}

	return stack->scopes[stack->count-1];
}

// Add a symbol to the table
scope_item *scope_stack_push(scope_stack *stack) {
	if (stack->capacity == 0) {
		stack->scopes = realloc(stack->scopes, sizeof *stack->scopes * (stack->count + 5));
		if (stack->scopes == NULL) {
			PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
		}
		stack->capacity += 5;
	}

	// The scope stack must have a global scope
	scope_item *item = scope_item_make();
	stack->scopes[stack->count++] = item;
	stack->capacity--;
	return item;
}

// Add a symbol to the table
void scope_stack_pop(scope_stack *stack) {
	if (stack->count == 1) {
		PANIC("Attempting to pop the file scope from the stack.");
	}
	free(stack->scopes[--stack->count]);
	stack->capacity++;
}
