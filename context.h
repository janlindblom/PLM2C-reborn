#include "defs.h"
#include "misc.h"
#include "struct.h"

void add_decl_to_context(DECL *decl);
void add_to_context(DECL_MEMBER *decl);
void init_context();
void new_context(int type, TOKEN *name);
void pop_context();

BOOLEAN find_member_symbol(TOKEN *symbol, DECL_MEMBER *decl_ptr, DECL_ID **decl_id);
BOOLEAN find_list_symbol(TOKEN *symbol, DECL_MEMBER *decl_ptr, DECL_MEMBER **decl_found, DECL_ID **decl_id);
BOOLEAN find_symbol(TOKEN *symbol, DECL_MEMBER **decl_found, DECL_ID **decl_id);
