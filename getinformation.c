#include "shell.h"

/**
 * clear_information - Initializes info_t struct
 * @info: Struct address
 */
void clear_information(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * setting_information - Initializes info_t struct
 * @info: Struct address
 * @av: Argument vector
 */
void setting_information(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = string_to_words(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _string_duplicate(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias_tkstr(info);
		replace_vars_tkstr(info);
	}
}

/**
 * frees_information - Frees info_t struct fields
 * @info: Struct address
 * @all: True if freeing all fields
 */
void frees_information(info_t *info, int all)
{
	frees_string(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			frees_allnodes_list(&(info->env));
		if (info->history)
			frees_allnodes_list(&(info->history));
		if (info->alias)
			frees_allnodes_list(&(info->alias));
		frees_string(info->environment);
		info->environment = NULL;
		free_pointer((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_put_character(BUF_FLUSH);
	}
}
