#include "shell.h"

/**
 * get_environment - Returns the string array copy of our environ.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
char **get_environment(info_t *info)
{
	if (!info->environment || info->env_changed)
	{
		info->environment = lists_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environment);
}

/**
 * _unsetenvironment - Removes an environment variable.
 * @info: Structure containing potential arguments.
 * @var: The string env var property.
 * Return: 1 on delete, 0 otherwise.
 */
int _unsetenvironment(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with_haystack(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = deletes_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenvironment - Initializes a new environment variable,
 *                   or modifies an existing one.
 * @info: Structure containing potential arguments.
 * @var: The string env var property.
 * @value: The string env var value.
 * Return: Always 0.
 */
int _setenvironment(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_string_length(var) + _string_length(value) + 2);
	if (!buf)
		return (1);
	_string_copy(buf, var);
	_string_concatenate(buf, "=");
	_string_concatenate(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with_haystack(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	adds_node_toend(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
