#include <stdbool.h>
#include "defs.h"
#include "misc.h"
#include "struct.h"

int  check_cvt_id(TOKEN *token, CVT_ID *cvt_id, char **cvt_string);
void check_eol();

void get_param_list(PARAM_LIST **param_head);

void out_ident(TOKEN *ident, DECL_MEMBER *decl, DECL_ID *decl_id);

void parse_call(TOKEN *first_token);
void parse_do(TOKEN *first_token);
void parse_else(TOKEN *first_token);
void parse_end(TOKEN *first_token);
void parse_eol(TOKEN *first_token);
int  parse_expression(TOKEN *token);
int  parse_function(TOKEN *token);
void parse_go(TOKEN *first_token);
void parse_goto(TOKEN *first_token);
void parse_identifier(TOKEN *first_token);
void parse_if(TOKEN *first_token);
void parse_int_ctl(TOKEN *first_token);
void parse_label();
int  parse_member(TOKEN *token, DECL_MEMBER *decl, DECL_ID *decl_id);
void parse_outhword(TOKEN *first_token);
void parse_output(TOKEN *first_token);
void parse_outword(TOKEN *first_token);
void parse_param_list(PARAM_LIST *param_list, DECL **decl_list, DECL **extra_decl_list);
void parse_return(TOKEN *first_token);
int  parse_simple_variable(TOKEN *token, TOKEN *next_token);
void parse_then();
void parse_till(int type, TOKEN *token);
void parse_till_end(TOKEN *token);
void parse_to_end();
int  parse_variable(TOKEN *token, DECL_MEMBER **var_decl, DECL_ID **var_decl_id);
