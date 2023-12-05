#include "shell.h"

/**
 * hsh - main shell loop
 * @info: parameter & return info struct
 * @av: argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_information(info);
		if (isInteractiveMode(info))
			_put_string("$ ");
		_errorputcharacter(BUF_FLUSH);
		r = gets_input(info);
		if (r != -1)
		{
			setting_information(info, av);
			builtin_ret = finds_builtin(info);
			if (builtin_ret == -1)
				finds_command(info);
		}
		else if (isInteractiveMode(info))
			_put_character('\n');
		frees_information(info, 0);
	}
	writes_history(info);
	frees_information(info, 1);
	if (!isInteractiveMode(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * finds_builtin - finds a builtin command
 * @info: parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int finds_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexiting},
		{"_environment", _myenvironment},
		{"help", _help},
		{"history", _history},
		{"_setenvironment", _mysetenvironment},
		{"_notsetenvironment", _mynotsetenvironment},
		{"cd", _mychangedirectory},
		{"alias", _ningialias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_string_comparison(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * finds_command - finds a command in PATH
 * @info: parameter & return info struct
 *
 * Return: void
 */
void finds_command(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!isDelimiter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = finds_path(info, _getenvironment(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forks_command(info);
	}
	else
	{
		if ((isInteractiveMode(info) || _getenvironment(info, "PATH=")
			|| info->argv[0][0] == '/') && is_command(info, info->argv[0]))
			forks_command(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			prints_errors(info, "not found\n");
		}
	}
}

/**
 * forks_command - forks a an exec thread to run cmd
 * @info: parameter & return info struct
 *
 * Return: void
 */
void forks_command(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environment(info)) == -1)
		{
			frees_information(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				prints_errors(info, "Permission denied\n");
		}
	}
}
