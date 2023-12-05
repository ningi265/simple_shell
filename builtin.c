#include "shell.h"

/**
 * _myexiting - Exits the shell.
 * @info: Structure containing potential arguments.
 *
 * Description:
 *   Exits the shell with a given exit status. If an exit argument exists,
 *   converts it to an integer using _stringtoint. If the conversion fails,
 *   prints an error message and sets the exit status to 2. If successful,
 *   sets info->err_num and returns -2.
 *
 * Return: 0 on success, 1 on error.
 */
int _myexiting(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit argument */
	{
		exitcheck = _stringtoint(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			prints_errors(info, "Illegal number: ");
			_input(info->argv[1]);
			_errorputcharacter('\n');
			return (1);
		}
		info->err_num = _stringtoint(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mychangedirectory - Changes the current directory of the process.
 * @info: Structure containing potential arguments.
 *
 * Description:
 *   Retrieves the current working directory using getcwd. If no argument is
 *   provided, attempts to change to the home directory. If argument is "-",
 *   changes to the previous working directory (if available). Otherwise,
 *   attempts to change to the specified directory. Updates the environment
 *   variables "OLDPWD" and "PWD" accordingly.
 *
 * Return: Always 0.
 */
int _mychangedirectory(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_put_string("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenvironment(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenvironment(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_string_comparison(info->argv[1], "-") == 0)
	{
		if (!_getenvironment(info, "OLDPWD="))
		{
			_put_string(s);
			_put_character('\n');
			return (1);
		}
		_put_string(_getenvironment(info, "OLDPWD=")), _put_character('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenvironment(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		prints_errors(info, "can't cd to ");
		_input(info->argv[1]), _errorputcharacter('\n');
	}
	else
	{
		_setenvironment(info, "OLDPWD", _getenvironment(info, "PWD="));
		_setenvironment(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _help - Displays help information.
 * @info: Structure containing potential arguments.
 *
 * Description:
 *   Displays a help message (currently a placeholder message).
 *   Takes array of arguments (info->argv) and prints placeholder message.
 *   The function is not fully implemented as per the comment.
 *
 * Return: Always 0.
 */
int _help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_put_string("help call works. Function not yet implemented \n");
	if (0)
		_put_string(*arg_array); /* temp att_unused workaround */
	return (0);
}
