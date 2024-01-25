#ifdef MODERN
#include "defs.h"
#include "misc.h"
#include "struct.h"

void add_decl_to_context(DECL *decl);
void add_to_context(DECL_MEMBER *decl);
void init_context();
void new_context(int type, TOKEN *name);
void pop_context();

int find_list_symbol(TOKEN *symbol, DECL_MEMBER **decl_ptr, DECL_MEMBER **decl_found, DECL_ID **decl_id);
int find_symbol(TOKEN *symbol, DECL_MEMBER **decl_found, DECL_ID **decl_id);
#endif