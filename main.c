#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef IBMPC
#include <sys\stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include "misc.h"
#include "defs.h"
#include "cvt.h"
#include "struct.h"
#include "tokens.h"
#include "tkn_defs.h"
#ifdef MODERN
#include "context.h"
#include "convert.h"
#include "error.h"
#include "io.h"
#include "mem.h"
#include "parse.h"
#include "token.h"
#endif

char	*text_buffer, *text_ptr;
int	line_count;
char	*line_ptr;
char	current_file_name[128];
char	out_file_name[128];
int	at_decl_count;
char	at_decl_list[MAX_AT_DECLS][MAX_TOKEN_LENGTH];
FILE	*ofd;

int	file_depth;
#ifndef MODERN
FILE	*fopen();
#endif
/*
 *	Get list of AT declaration variables for EXTERNAL declaration checks
 */
void get_at_decl()
{
#ifdef MODERN
	int i;
	FILE *fd;
#else
	int	i, fd;
#endif
	char	ch;

	at_decl_count = 0;
#ifdef MODERN
	if ((fopen_s(&fd, "at_decl.cvt", O_RDONLY)) == -1)
#else
	if ((fd = open("at_decl.cvt", O_RDONLY)) == -1)
#endif
			/* Not found */
		return;
#ifdef MODERN
	while (fread(&ch, 1, 1, fd) == 1)
#else
	while (read(fd, &ch, 1) == 1)
#endif
	{
	    i = 0;
	    if (!is_a_char(ch)) {
		fprintf(stderr, "Illegal identifier in line %d at_decl.cvt\n",
			at_decl_count + 1);
		exit(1);
	    }
	    do {
#ifdef CONVERT_CASE
		if (is_a_uc_char(ch))
				/* Convert to lower case */
			ch += 32;
		else
		if (is_a_lc_char(ch))
				/* Convert to upper case */
			ch -= 32;
#endif
		at_decl_list[at_decl_count][i++] = ch;
#ifdef MODERN
		if (fread(&ch, 1, 1, fd) != 1)
#else
		if (read(fd, &ch, 1) != 1)
#endif
		{
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
#ifdef MODERN
void cvt_file(char *file_name)
#else
void cvt_file(file_name)
char	*file_name;
#endif
{
#ifdef MODERN
	int nr;
	FILE *fd;
#else
	int		fd, nr;
#endif
	struct	stat	file_stat;
	TOKEN		token, fname_token, token_module, token_do;
	int		token_class;
	char		*tmp_text_buffer, *tmp_text_ptr, *tmp_line_ptr;
	char		*tmp_ptr;
	int		tmp_line_count;
	char		tmp_file_name[128];
#ifndef MODERN
	char		*get_mem();
#endif
		/* Is this the first file? */
	if (file_depth) {
			/* No - save old text pointers */
		tmp_text_buffer = text_buffer;
		tmp_text_ptr = text_ptr;
		tmp_line_ptr = line_ptr;
		tmp_line_count = line_count;
#ifdef MODERN
		strcpy_s(tmp_file_name, 128, current_file_name);
#else
		(void) strcpy(tmp_file_name, current_file_name);
#endif
	}

		/* Save file name */
#ifdef MODERN
	strcpy_s(current_file_name, 128, file_name);
#else
	(void) strcpy(current_file_name, file_name);
#endif

		/* Open file */
#ifdef MODERN
	if (fopen_s(&fd, file_name, O_RDONLY) == -1)
#else
	if ((fd = open(file_name, O_RDONLY)) == -1)
#endif
	{
		(void) fprintf(stderr, "Cannot open input file %s", file_name);
		perror("");
		exit(1);
	}

		/* Get length */
#ifdef MODERN
	if (stat(file_name, &file_stat))
#else
	if (fstat(fd, &file_stat))
#endif
	{
		perror("Cannot stat input file");
		exit(1);
	}

		/* Allocate that much RAM */
	text_buffer = get_mem((unsigned int) file_stat.st_size + 1);

		/* Read file */
#ifdef MODERN
	if ((nr = fread(text_buffer, 1, (int) file_stat.st_size, fd)) == -1)
#else
	if ((nr = read(fd, text_buffer, (int) file_stat.st_size)) == -1)
#endif
	{
		perror("Cannot read input file");
		exit(1);
	}

		/* Insert End-of-file Mark */
	text_buffer[nr] = '\0';
#ifdef MODERN
	fclose(fd);
#else
	(void) close(fd);
#endif

		/* Init pointers */
	text_ptr = text_buffer;
	line_ptr = text_ptr;
	line_count = 1;

		/* Init I/O */
	out_init();

		/* Start with initial context using file name */
#ifdef MODERN
	strcpy_s(fname_token.token_name, 512, file_name);
#else
	(void) strcpy(fname_token.token_name, file_name);
#endif
	fname_token.token_class = IDENTIFIER;
	new_context(MODULE, &fname_token);

		/* Is this the first file? */
	if (file_depth++ == 0) {
			/* Yes - open output file */
#ifdef MODERN
		if (fopen_s(&ofd, out_file_name, "w") == -1)
#else
		if ((ofd = fopen(out_file_name, "w")) == NULL)
#endif
		{
			(void) fprintf(stderr, "Cannot create output file %s",
				out_file_name);
			exit(1);
		}

			/* Check for module name */
		token_class = get_token(&token_module);
		out_pre_white(&token_module);
		tmp_ptr = token_module.token_start;
		if ((token_class == IDENTIFIER) &&
				/* Maybe got module name - Check for : */
		    (get_token(&token) == LABEL) &&
				/* Check for DO; */
		   ((get_token(&token_do) == RESERVED) &&
					(token_do.token_type == DO)) &&
		    (get_token(&token) == END_OF_LINE)) {

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
	} else
		parse_file();

	free(text_buffer);

		/* Was this the first file? */
	if (--file_depth) {
			/* No - restore old text pointers */
		text_buffer = tmp_text_buffer;
		text_ptr = tmp_text_ptr;
		line_ptr = tmp_line_ptr;
		line_count = tmp_line_count;
#ifdef MODERN
		strcpy_s(current_file_name, 128, tmp_file_name);
#else
		(void) strcpy(current_file_name, tmp_file_name);
#endif
	} else
		exit(0);
}

/*
 *	Open file and init options
 */
#ifdef MODERN
int main(int argc, char *argv[])
#else
int main(argc, argv)
int	argc;
char	*argv[];
#endif
{
	int	i;
	char	ch;

	if (argc != 2) {
		(void) fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}

		/* Search for a '.' in filename */
	for (i = strlen(argv[1]) - 1; i; i--) {
		ch = argv[1][i];
		if ((ch == '.') || (ch == '/') || (ch == '\\'))
			break;
	}

	if (ch != '.')
		i = strlen(argv[1]);

		/* Append a '.c' */
#ifdef MODERN
	//strncpy(out_file_name, argv[1], i);
	strncpy_s(out_file_name, 128, argv[1], i);
#else
	(void) strncpy(out_file_name, argv[1], i);
#endif
	out_file_name[i] = '\0';
#ifdef MODERN
	strcat_s(out_file_name, 128, ".c");
#else
	(void) strcat(out_file_name, ".c");
#endif
	(void) printf("Output to: %s\n", out_file_name);

		/* Get AT declaration list */
	get_at_decl();

		/* Init context */
	init_context();

	file_depth = 0;

		/* Parse main file */
	cvt_file(argv[1]);
	return 0;
}
