#include "defs.h"
#include "misc.h"
#include "struct.h"

void cvt_octal(TOKEN *token, char *octal_string);

void out_char(char ch);
void out_cvt_name(TOKEN *token);
void out_data(char *string, int length);
void out_init();
void out_must_white(TOKEN *token);
void out_pre_line(TOKEN *token);
void out_pre_white(TOKEN *token);
void out_str(char *string);
void out_str_const(char *str_ptr, int len);
void out_to_start();
void out_token(TOKEN *token);
void out_token_name(TOKEN *token);
void out_type(int type);
void out_white_space(TOKEN *token);
