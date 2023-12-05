#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environment: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environment;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int hsh(info_t *, char **);
int finds_builtin(info_t *);
void finds_command(info_t *);
void forks_command(info_t *);

/* toem_parser.c */
int is_command(info_t *, char *);
char *duplicates_characters(char *, int, int);
char *finds_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _input(char *);
int _errorputcharacter(char);
int _putcharacterfd(char c, int fd);
int _putstringfd(char *str, int fd);

/* toem_string.c */
int _string_length(char *);
int _string_comparison(char *, char *);
char *starts_with_haystack(const char *, const char *);
char *_string_concatenate(char *, char *);

/* toem_string1.c */
char *_string_copy(char *, char *);
char *_string_duplicate(const char *);
void _put_string(char *);
int _put_character(char);

/* toem_exits.c */
char *_stringcopy(char *, char *, int);
char *_stringconcatenate(char *, char *, int);
char *_stringcharacter(char *, char);

/* toem_tokenizer.c */
char **string_to_words(char *, char *);
char **string_to_words2(char *, char);

/* toem_realloc.c */
char *_memorysetting(char *, char, unsigned int);
void frees_string(char **);
void *_reallocation(void *, unsigned int, unsigned int);

/* toem_memory.c */
int free_pointer(void **);

/* toem_atoi.c */
int isInteractiveMode(info_t *);
int isDelimiter(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _stringtoint(char *);
void prints_errors(info_t *, char *);
int prints_decimal(int, int);
char *converts_numbers(long int, int, int);
void removes_comments(char *);

/* toem_builtin.c */
int _myexiting(info_t *);
int _mychangedirectory(info_t *);
int _help(info_t *);

/* toem_builtin1.c */
int _history(info_t *);
int _ningialias(info_t *);

/*toem_getline.c */
ssize_t gets_input(info_t *);
int _gets_next_line(info_t *, char **, size_t *);
void signintHandler(int);

/* toem_getinfo.c */
void clear_information(info_t *);
void setting_information(info_t *, char **);
void frees_information(info_t *, int);

/* toem_environ.c */
char *_getenvironment(info_t *, const char *);
int _myenvironment(info_t *);
int _mysetenvironment(info_t *);
int _mynotsetenvironment(info_t *);
int populate_environment_list(info_t *);

/* toem_getenv.c */
char **get_environment(info_t *);
int _notsetenvironment(info_t *, char *);
int _setenvironment(info_t *, char *, char *);

/* toem_history.c */
char *gets_history_file(info_t *info);
int writes_history(info_t *info);
int reads_history(info_t *info);
int builds_history_list(info_t *info, char *buf, int linecount);
int renumbers_history(info_t *info);

/* toem_lists.c */
list_t *adds_node_tostart(list_t **, const char *, int);
list_t *adds_node_toend(list_t **, const char *, int);
size_t prints_lists_string(const list_t *);
int deletes_node_at_index(list_t **, unsigned int);
void frees_allnodes_list(list_t **);

/* toem_lists1.c */
size_t linked_list_length(const list_t *);
char **lists_to_strings(list_t *);
size_t prints_linked_list(const list_t *);
list_t *node_starting_with(list_t *, char *, char);
ssize_t gets_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain_delimeter(info_t *, char *, size_t *);
void check_chain_status(info_t *, char *, size_t *, size_t, size_t);
int replace_alias_tkstr(info_t *);
int replace_vars_tkstr(info_t *);
int replaces_string(char **, char *);

#endif
