#include "misc.h"
#include "defs.h"
#include "struct.h"

void element_copy(DECL_MEMBER *src, DECL_MEMBER *dest);
void find_right_paren();
void get_decl_list(DECL *decl);
int get_var_list(DECL_ID **list_ptr, TOKEN *sep_token);
void parse_declare(TOKEN *first_token);
int parse_type(DECL_MEMBER *el_ptr, TOKEN *token);
