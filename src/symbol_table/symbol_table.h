#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS_PER_SCOPE 100

/* One declared identifier */
typedef struct Symbol {
    char name[64];
    char type[16];   /* "int" | "float" | "bool" */
    int line;        /* line number where declared */
} Symbol;

/* One scope (block). Scopes are chained into a stack via 'parent'. */
typedef struct Scope {
    Symbol symbols[MAX_SYMBOLS_PER_SCOPE];
    int symbol_count;
    struct Scope *parent;
} Scope;

/* Global current-scope pointer, managed by these functions */
void scope_init(void);                 /* create the global/top scope */
void scope_enter(void);                /* push a new nested scope */
void scope_exit(void);                 /* pop the current scope */

/* Returns 1 on success, 0 if 'name' is already declared in the CURRENT scope
   (redeclaration error) */
int scope_declare(const char *name, const char *type, int line);

/* Searches the current scope AND all enclosing (parent) scopes.
   Returns the type string ("int"/"float"/"bool") if found, or NULL if
   the identifier was never declared in any visible scope. */
const char *scope_lookup(const char *name);

/* Prints all symbols in the current scope chain (for debugging / demo) */
void scope_print_all(Scope *s);

#endif
