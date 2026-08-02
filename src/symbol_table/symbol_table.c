#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* The scope stack is simply a linked list; 'current' is the innermost
   (most deeply nested) scope currently active. */
static Scope *current = NULL;

void scope_init(void) {
    current = (Scope *)malloc(sizeof(Scope));
    current->symbol_count = 0;
    current->parent = NULL;
}

void scope_enter(void) {
    Scope *s = (Scope *)malloc(sizeof(Scope));
    s->symbol_count = 0;
    s->parent = current;
    current = s;
}

void scope_exit(void) {
    if (current == NULL) return;
    Scope *old = current;
    current = current->parent;
    free(old);
}

int scope_declare(const char *name, const char *type, int line) {
    /* Redeclaration check: only look in the CURRENT scope, not parents.
       (A variable in an outer scope CAN be re-declared inside an inner
       block -- that's normal shadowing, not an error.) */
    for (int i = 0; i < current->symbol_count; i++) {
        if (strcmp(current->symbols[i].name, name) == 0) {
            return 0; /* already declared in this exact scope */
        }
    }
    if (current->symbol_count >= MAX_SYMBOLS_PER_SCOPE) {
        return 0; /* scope full -- treat as failure */
    }
    strncpy(current->symbols[current->symbol_count].name, name, 63);
    strncpy(current->symbols[current->symbol_count].type, type, 15);
    current->symbols[current->symbol_count].line = line;
    current->symbol_count++;
    return 1;
}

const char *scope_lookup(const char *name) {
    Scope *s = current;
    while (s != NULL) {
        for (int i = 0; i < s->symbol_count; i++) {
            if (strcmp(s->symbols[i].name, name) == 0) {
                return s->symbols[i].type;
            }
        }
        s = s->parent; /* not found here -- check the enclosing scope */
    }
    return NULL; /* not declared anywhere visible */
}

void scope_print_all(Scope *s) {
    if (s == NULL) s = current;
    int depth = 0;
    Scope *tmp = s;
    while (tmp) { depth++; tmp = tmp->parent; }

    printf("\n=== Symbol Table (innermost scope first) ===\n");
    printf("%-6s %-20s %-10s %-6s\n", "Scope", "Name", "Type", "Line");
    tmp = s;
    int level = depth;
    while (tmp) {
        for (int i = 0; i < tmp->symbol_count; i++) {
            printf("%-6d %-20s %-10s %-6d\n", level,
                   tmp->symbols[i].name, tmp->symbols[i].type, tmp->symbols[i].line);
        }
        tmp = tmp->parent;
        level--;
    }
}
