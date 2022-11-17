#include "shell.h"
/**
 * alias_func - deals with command aliases
 * @args: arguments from command line
 * @to_free: indicated if aliases need to be freed (exiting shell);
 * Return: TRUE if exiting, FALSE if the command is not "alias" or an alias,
 * SKIP_FORK if success
 */
int alias_func(char **args, int to_free)
{
	int status;
	static alias head = {NULL, NULL, NULL};
	char *char_ptr;
	int no_error = TRUE;

	if (to_free == TRUE)
		return (free_aliases(head.next));

	if (str_compare("alias", *args, MATCH) != TRUE)
		return (check_if_alias(args, head.next));
	args++;

	if (*args == NULL)
		return (print_aliases(head.next));

	while (*args != NULL)
	{
		char_ptr = *args;
		while (*char_ptr != '\0' && *char_ptr != '=')
			char_ptr++;

		if (*char_ptr == '\0' || char_ptr == *args)
		{
			if (print_alias_value(*args, &head) == FALSE)
				no_error = FALSE;
		}
		else
		{
			*char_ptr = '\0';
			char_ptr++;
			set_alias_value(*args, &head, char_ptr);
			*(char_ptr - 1) = '=';
		}
		args++;
	}
	if (no_error == FALSE)
		return (SKIP_FORK);

	status = 0;
	return (SKIP_FORK);
}
/**
 * _setenv - sets and environmental variable
 * @name: name of the variable
 * @value: value to set the variable to
 * Return: 0 on success
 */
int _setenv(const char *name, const char *value)
{
	int status;
	char **new_environ;
	char *buffer;
	char *buf_tmp;
	char *element_ptr;
	int len;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return (SKIP_FORK);
	}

	buffer = str_concat((char *)name, "=");

	element_ptr = get_array_element(environ, buffer);

	buf_tmp = str_concat(buffer, (char *)value);
	free(buffer);
	buffer = buf_tmp;

	if (element_ptr == NULL)
	{
		len = list_len(environ, NULL);
		new_environ = array_cpy(environ, len + 1);
		new_environ[len - 1] = buffer;
		new_environ[len] = NULL;
		free_array(environ);
		environ = new_environ;
		return (SKIP_FORK);
	}

	len = list_len(environ, (char *)name);
	free(environ[len]);
	environ[len] = buffer;

	status = 0;

	return (SKIP_FORK);
}
/**
 * _unsetenv - deletes an environmental variable
 * @name: name of variable
 * Return: 0 if successful
 */
int _unsetenv(const char *name)
{
	int status;
	char **env_ptr;
	char *buffer;
	int len;

	buffer = str_concat((char *)name, "=");
	len = list_len(environ, buffer);
	free(buffer);

	if (len == -1)
	{
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		status = 2;
		return (SKIP_FORK);
	}

	env_ptr = environ + len;
	free(*env_ptr);
	while (*(env_ptr + 1) != NULL)
	{
		*env_ptr = *(env_ptr + 1);
		env_ptr++;
	}
	*env_ptr = NULL;
	status = 0;

	return (SKIP_FORK);
}
/**
 * change_dir - changes the current working directory
 * @name: name of directory to change to
 * Return: 0 if successful
 */
int change_dir(char *name)
{
	int status, i;
	char *home, *pwd;
	char old_path_buffer[PATH_MAX], new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;

	getcwd(path_buffer, buf_size);
	if (name == NULL || str_compare("~", name, PREFIX) == TRUE
			|| str_compare("$HOME", name, MATCH) == TRUE)
	{
		if ((name != NULL && *name == '~' && *(name + 1) != '\0'
					&& *(name + 1) != '/')
			{
				status = 2;
				err_message("cd", name);
				return (SKIP_FORK);
			}
		home = get_array_element(environ, "HOME=");
		if (home == NULL)
		{
			status = 2;
			err_message("cd", name);
			return (SKIP_FORK);
		} while (*home != '=')
			home++;
		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);
		if (name != NULL && *(name + 1) != '\0' && *(name + 2) != '\0')
			name += 2;
	} else if (str_compare("-", name, MATCH) == TRUE)
	{
		pwd = get_array_element(environ, "OLDPWD=");
		if (pwd == NULL)
			return (2);
		while (*pwd != '=')
			pwd++;

		i = chdir((const char *)pwd);
		if (i != -1)
		{
			write(STDOUT_FILENO, pwd, _strlen(pwd));
			write(STDOUT_FILENO, "\n", 1);
			_setenv("PWD", (const char *)pwd);
		}
	} else if (name != NULL)
	{
		i = chdir((const char *)name);
		if (i != -1)
			_setenv("PWD", getcwd(new_path_buffer, buf_size));
	}
	if (i == -1)
	{
		status = 2;
		err_message("cd", name);
		return (SKIP_FORK);
	}
	status = 0;
	_setenv("OLDPWD", (const char *)path_buffer);
	return (SKIP_FORK);
}
/**
 * print_env - prints the environment
 * Return: TRUE
 */
int print_env(void)
{
	int status;
	char **ptr = environ;

	while (*ptr != NULL)
	{
		write(STDOUT_FILENO, *ptr, _strlen(*ptr));
		write(STDOUT_FILENO, "\n", 1);
		ptr++;
	}

	status = 0;
	return (SKIP_FORK);
}
