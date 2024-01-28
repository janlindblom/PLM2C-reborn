#include <stdbool.h>
#include <stdint.h>
#include "tokens.h"
/*
 *	Miscellaneous defines
 */

typedef bool BOOLEAN;

#ifndef TRUE
#    define TRUE true
#endif

#ifndef FALSE
#    define FALSE false
#endif

#ifndef NULL
#    define NULL 0
#endif

/*
 *	White space characters
 */
#define SPACE ' '
#define TAB 9
#define CR 13
#define LF 10

/*
 *	Useful defines
 */
#define is_a_uc_char(char) ((char >= 'A') && (char <= 'Z'))
#define is_a_lc_char(char) ((char >= 'a') && (char <= 'z'))
#define is_a_char(char) (((char & 0x5F) >= 'A') && ((char & 0x5F) <= 'Z'))
#define is_a_digit(char) ((char >= '0') && (char <= '9'))

#define is_a_type(token) ((token->token_class == RESERVED) && (token->token_type >= BYTE) && (token->token_type <= REAL))

#define is_white(ch) ((ch == ' ') || (ch == TAB))

#define NONE 0

void cvt_file(char *file_name);
