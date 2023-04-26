#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

#include <stdbool.h>

// Define the symbol table entry structure
typedef struct {
	Cstr identifier;
	bool takes_args;
	Cstr_Array args;
	Cstr_Array replacement;
} macro_definition;

macro_definition *macro_definitione_make() {
	macro_definition *def = malloc(sizeof *def);
	if (def == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	memset(def, 0, sizeof *def);
	def->takes_args = false;
	def->args = cstr_array_make(NULL);
	def->replacement = cstr_array_make(NULL);
	return def;
}

void macro_definition_set_identifier(macro_definition *def, Cstr identifier) {
	size_t id_len = strlen(identifier);
	char *temp = malloc(id_len+1);
	if (temp == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}
	def->identifier = strncpy(temp, identifier, id_len+1);
}

void macro_definition_push_args(macro_definition *def, Cstr token) {
	if (!def->takes_args) {
		PANIC("Macro definition '%s' does not take args.", def->identifier);
	}
	cstr_array_append(def->replacement, token);
}

void macro_definition_push_replacement(macro_definition *def, Cstr token) {
	cstr_array_append(def->replacement, token);
}

// Define the symbol table structure
typedef struct {
	macro_definition **macros;
	size_t count;
	size_t capacity;
} macro_table;

macro_table *macro_table_make() {
	macro_table *table = malloc(sizeof *table);
	if (table == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	memset(table, 0, sizeof *table);
	table->capacity = 10;
	table->macros = malloc(sizeof *table->macros * table->capacity);
	if (table->macros == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	return table;
}

macro_definition *macro_table_peek(macro_table *table) {
	if (table->count == 0) {
		PANIC("Attempting to peek an empty symbol table.");
	}

	return table->macros[table->count-1];
}

// Add a symbol to the table
void macro_table_push(macro_table *table, Cstr identifier) {
	size_t macro_len = strlen(identifier);
	// Check if the symbol already exists
	for (size_t i = 0; i < table->count; ++i) {
		size_t table_macro_len = strlen(table->macros[i]->identifier);
		if (macro_len == table_macro_len && strcmp(identifier, table->macros[i]->identifier) == 0) {
			// Move the macro to end of the array so that it can be peeked
			macro_definition *def = table->macros[i];
			for (size_t j = i; j < table->count - 1; ++j) {
				table->macros[j] = table->macros[j+1];
			}
			table->macros[table->count-1] = def;

			return; // Symbol already exists
		}
	}

	macro_definition *macro = macro_definitione_make();
	macro_definition_set_identifier(macro, identifier);

	if (table->count == table->capacity) {
		table->macros = realloc(table->macros, sizeof *table->macros * (table->count + 5));
		if (table->macros == NULL) {
			PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
		}
		table->capacity += 5;
	}

	table->macros[table->count++] = macro;
}

// Add a symbol to the table
void macro_table_remove(macro_table *table, Cstr identifier) {
	size_t macro_len = strlen(identifier);
	// Check if the symbol already exists
	for (size_t i = 0; i < table->count; ++i) {
		size_t table_macro_len = strlen(table->macros[i]->identifier);
		if (macro_len != table_macro_len || strcmp(identifier, table->macros[i]->identifier) != 0) {
			continue;
		}

		free(table->macros[i]);
		for (size_t j = i; j < table->count - 1; ++j) {
			table->macros[j] = table->macros[j+1];
		}
		table->capacity--;
		table->count++;
		return;
	}
}

// Get the value of a symbol
macro_definition *macro_table_get_def(macro_table *table, char *identifier) {
	size_t id_len = strlen(identifier);
	for (size_t i = 0; i < table->count; i++) {
		if (id_len == strlen(table->macros[i]->identifier) && strcmp(identifier, table->macros[i]->identifier) == 0) {
			return table->macros[i];
		}
	}

	return NULL;
}
