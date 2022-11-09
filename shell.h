#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#define FALSE 0
#define TRUE 1
#define NEITHER 2
#define MATCH 3
#define PREFIX 4
#define EXIT_SHELL 5
#define SKIP_FORK 6
#define DO_EXECVE 7

/**
 * struct Alias - singly linked list
 * @name: name of alias
 * @value: command that alias calls
 * @next: points to next node
 */
typedef struct Alias
{
	char *name;
	char *value;
	struct Alias *next;
} alias;

extern char *shell_name;
extern int status;
extern int line_num;
extern char **environ;

int command_manager(char **args);
char *_strdup(char *src);
int list_len(char **list, char *entry);
char *input_san(char *old_buf, size_t *old_size);
char *get_array_element(char **array, char *element_name);
char **make_array(char *str, char delim, char **if_sep);
char **array_cpy(char **old_array, int new_size);
int free_array(char **args);
int alias_func(char **args, int free);
int _getline(char **line_ptr, size_t *n, int file);
int str_compare(char *s1, char *s2, int pref_or_match);
int _strlen(char *str);
int input_err_check(char *ptr);
void err_message(char *arg0, char *arg1);
char *_itoa(int n);
int free_aliases(alias *alias_ptr);
int check_if_alias(char **args, alias *alias_ptr);
int print_aliases(alias *alias_ptr);
int print_alias_value(char *arg, alias *alias_ptr);
int set_alias_value(char *arg, alias *alias_ptr, char *new_value);

#endif
