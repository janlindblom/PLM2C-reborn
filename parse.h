#ifdef MODERN
#include "defs.h"
#include "misc.h"
#include "struct.h"

void parse_do(TOKEN *first_token);
void parse_if(TOKEN *first_token);
void parse_then();
void parse_else(TOKEN *first_token);
void parse_goto(TOKEN *first_token);
void parse_go(TOKEN *first_token);
void parse_call(TOKEN *first_token);
void parse_return(TOKEN *first_token);
void parse_end(TOKEN *first_token);
void parse_int_ctl(TOKEN *first_token);
void parse_identifier(TOKEN *first_token);
void parse_till_end(TOKEN *token);
#endif