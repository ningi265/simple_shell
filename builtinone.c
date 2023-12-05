#include "shell.h"

/**
 * _history - displays the history list, one command per line,
 *             with line numbers starting at 0.
 * @info: Structure containing potential arguments.
 *        Used to maintain constant function prototype.
 * Return: Always 0
 */
int _history(info_t *info)
{
	prints_linked_list(info->history);
	return (0);
}

/**
 * notset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int notset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _stringcharacter(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = deletes_node_at_index(&(info->alias),
		gets_node_index(info->alias, node_starting_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * setting_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int setting_alias(info_t *info, char *str)
{
	char *p;

	p = _stringcharacter(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (notset_alias(info, str));

	notset_alias(info, str);
	return (adds_node_toend(&(info->alias), str, 0) == NULL);
}

/**
 * printing_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int printing_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _stringcharacter(node->str, '=');
		for (a = node->str; a <= p; a++)
			_put_character(*a);
		_put_character('\'');
		_put_string(p + 1);
		_put_string("'\n");
		return (0);
	}
	return (1);
}

/**
 * _ningialias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments.
 *        Used to maintain constant function prototype.
 * Return: Always 0
 */
int _ningialias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			printing_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _stringcharacter(info->argv[i], '=');
		if (p)
			setting_alias(info, info->argv[i]);
		else
			printing_alias(node_starting_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
