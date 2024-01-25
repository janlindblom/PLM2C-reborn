#ifdef MODERN
#include "defs.h"
#include "misc.h"
#include "struct.h"

void free_decl(DECL *decl_list);
void free_decl_list(DECL_MEMBER *element);
void free_param_list(PARAM_LIST *param_list);
void free_var_list(DECL_ID *list_ptr);

char *get_mem(unsigned int size);

void get_decl_ptr(DECL **decl);
void get_element_ptr(DECL_MEMBER **element);
void get_param_ptr(PARAM_LIST **param);
void get_token_ptr(TOKEN **token);
void get_var_ptr(DECL_ID **var);
#endif