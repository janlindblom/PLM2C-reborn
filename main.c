#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "misc.h"
#include "defs.h"
#include "cvt.h"
#include "struct.h"
#include "tokens.h"
#include "tkn_defs.h"
#include "context.h"
#include "convert.h"
#include "error.h"
#include "io.h"
#include "mem.h"
#include "parse.h"
#include "token.h"

char *text_buffer, *text_ptr;
int   line_count;
char *line_ptr;
char  current_file_name[128];
char  out_file_name[128];
int   at_decl_count;
char  at_decl_list[MAX_AT_DECLS][MAX_TOKEN_LENGTH];
FILE *ofd;

int file_depth;

/*
 *	Get list of AT declaration variables for EXTERNAL declaration checks
 */
void get_at_decl() {
    int   i;
    FILE *fd;
    char  ch;

    at_decl_count = 0;
#ifdef _WIN32
    if ((fopen_s(&fd, "at_decl.cvt", O_RDONLY)) != 0)
#else
    fd = fopen("at_decl.cvt", "r");
    if (fd == NULL)
#endif
    {
        /* Not found */
        return;
    }
    while (fread(&ch, 1, 1, fd) == 1) {
        i = 0;
        if (!is_a_char(ch)) {
            fprintf(stderr, "Illegal identifier in line %d at_decl.cvt\n", at_decl_count + 1);
            exit(1);
        }
        do {
#ifdef CONVERT_CASE
            if (is_a_uc_char(ch)) {
                /* Convert to lower case */
                ch += 32;
            } else if (is_a_lc_char(ch)) {
                /* Convert to upper case */
                ch -= 32;
            }
#endif
            at_decl_list[at_decl_count][i++] = ch;
            if (fread(&ch, 1, 1, fd) != 1) {
                fprintf(stderr, "Unexpected EOF in at_decl.cvt\n");
                exit(1);
            }
        } while ((ch != '\n') && (ch != ' '));

        at_decl_list[at_decl_count++][i] = '\0';
    }
}

/*
 *	Open specified file, init options, and parse.
 */
void cvt_file(char *file_name) {
    int   nr;
    FILE *fd;

    struct stat file_stat;
    TOKEN       token, fname_token, token_module, token_do;
    int         token_class;
    char       *tmp_text_buffer, *tmp_text_ptr, *tmp_line_ptr;
    char       *tmp_ptr;
    int         tmp_line_count;
    char        tmp_file_name[128];

    /* Is this the first file? */
    if (file_depth) {
        /* No - save old text pointers */
        tmp_text_buffer = text_buffer;
        tmp_text_ptr    = text_ptr;
        tmp_line_ptr    = line_ptr;
        tmp_line_count  = line_count;
#ifdef _WIN32
        strcpy_s(tmp_file_name, sizeof(tmp_file_name), current_file_name);
#else
        strcpy(tmp_file_name, current_file_name);
#endif
    }

    /* Save file name */

#ifdef _WIN32
    strcpy_s(current_file_name, sizeof(current_file_name), file_name);
#else
    strcpy(current_file_name, file_name);
#endif

    /* Open file */

#ifdef _WIN32
    if ((fopen_s(&fd, file_name, "r")) != 0)
#else
    fd = fopen(file_name, "r");
    if (fd == NULL)
#endif
    {
        fprintf(stderr, "Cannot open input file %s: ", file_name);
        perror("");
        exit(1);
    }

    /* Get length */
    if (stat(file_name, &file_stat)) {
        perror("Cannot stat input file");
        exit(1);
    }

    /* Allocate that much RAM */
    text_buffer = get_mem((unsigned int)file_stat.st_size + 1);

    /* Read file */

    nr = fread(text_buffer, 1, (int)file_stat.st_size, fd);
    if (nr == -1) {
        perror("Cannot read input file");
        exit(1);
    }

    /* Insert End-of-file Mark */
    text_buffer[nr] = '\0';
    fclose(fd);

    /* Init pointers */
    text_ptr   = text_buffer;
    line_ptr   = text_ptr;
    line_count = 1;

    /* Init I/O */
    out_init();

    /* Start with initial context using file name */

#ifdef _WIN32
    strcpy_s(fname_token.token_name, sizeof(fname_token.token_name), file_name);
#else
    strcpy(fname_token.token_name, file_name);
#endif
    fname_token.token_class = IDENTIFIER;
    new_context(MODULE, &fname_token);

    /* Is this the first file? */
    if (file_depth++ == 0) {
/* Yes - open output file */
#ifndef DEBUG
#    ifdef _WIN32
        if (fopen_s(&ofd, out_file_name, "w") != 0)
#    else
        fd = fopen(out_file_name, "w");
        if (fd == NULL)
#    endif
        {
            (void)fprintf(stderr, "Cannot create output file %s", out_file_name);
            exit(1);
        }
#endif

        /* Check for module name */
        token_class = get_token(&token_module);
        out_pre_white(&token_module);

        tmp_ptr = token_module.token_start;
        if ((token_class == IDENTIFIER) &&
            /* Maybe got module name - Check for : */
            (get_token(&token) == LABEL) &&
            /* Check for DO; */
            ((get_token(&token_do) == RESERVED) && (token_do.token_type == DO)) && (get_token(&token) == END_OF_LINE)) {
            /* Got module header */
            out_pre_white(&token_do);

            /* Parse to END [<module name>] */
            parse_till_end(&token);
            out_white_space(&token);

            token_class = get_token(&token);
            if (token_class == IDENTIFIER) {
                out_pre_white(&token);
                token_class = get_token(&token);
            }

            /* Should be at end of line */
            if (token_class != END_OF_LINE) {
                parse_error("';' expected");
            }

            /* Should be at end of file */
            if (get_token(&token) != END_OF_FILE) {
                parse_error("End of file expected");
            }
            out_white_space(&token);
        } else {
            out_pre_white(&token_do);
            parse_warning("Module name expected");
            text_ptr = tmp_ptr;
            parse_file();
        }
    } else {
        parse_file();
    }

    free(text_buffer);

    /* Was this the first file? */
    if (--file_depth) {
        /* No - restore old text pointers */
        text_buffer = tmp_text_buffer;
        text_ptr    = tmp_text_ptr;
        line_ptr    = tmp_line_ptr;
        line_count  = tmp_line_count;
#ifdef _WIN32
        strcpy_s(current_file_name, sizeof(current_file_name), tmp_file_name);
#else
        strcpy(current_file_name, tmp_file_name);
#endif
    } else {
        exit(0);
    }
}

/*
 *	Open file and init options
 */
int main(int argc, char *argv[]) {
    int  i;
    char ch;

    if (argc < 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    /* Search for a '.' in filename */
    for (i = strlen(argv[1]) - 1; i; i--) {
        ch = argv[1][i];
        if ((ch == '.') || (ch == '/') || (ch == '\\')) {
            break;
        }
    }

    if (ch != '.') {
        i = strlen(argv[1]);
    }
#ifndef DEBUG
    /* Append a '.c' */

#    ifdef _WIN32
    strncpy_s(out_file_name, sizeof(out_file_name), argv[1], i);
#    else
    strncpy(out_file_name, argv[1], i);
#    endif
    out_file_name[i] = '\0';
#    ifdef _WIN32
    strcat_s(out_file_name, sizeof(out_file_name), ".c");
#    else
    strcat(out_file_name, ".c");
#    endif

    fprintf(stderr, "Output to: %s\n", out_file_name);
#endif

    /* Get AT declaration list */
    get_at_decl();

    /* Init context */
    init_context();

    file_depth = 0;

    /* Parse main file */
    cvt_file(argv[1]);
    return 0;
}
