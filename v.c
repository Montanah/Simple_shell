int status, i;
	char *home, *pwd, old_p_buffer[PATH_MAX], new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;

	pwd = getcwd(old_p_buffer, buf_size);
	if (name == NULL || str_compare("~", name, PREFIX) == TRUE
			|| str_compare("$HOME", name, MATCH) == TRUE)
	{
		if (name != NULL && *name == '~' && *(name + 1) != '\0'
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
	_setenv("OLDPWD", (const char *)old_p_buffer);
	return (SKIP_FORK);
}
/**