#include "shell.h"

/**
 * _myenvironment - Prints the current environment.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int _myenvironment(info_t *info)
{
	prints_lists_string(info->env);
	return (0);
}

/**
 * _getenvironment - Gets the value of an environment variable.
 * @info: Structure containing potential arguments.
 * @name: Environment variable name.
 * Return: The value of the environment variable.
 */
char *_getenvironment(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with_haystack(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenvironment - Initializes a new environment variable
 *                    or modifies an existing one.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int _mysetenvironment(info_t *info)
{
	if (info->argc != 3)
	{
		_input("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenvironment(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _mynotsetenvironment - Removes an environment variable.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int _mynotsetenvironment(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_input("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_notsetenvironment(info, info->argv[i]);

	return (0);
}

/**
 * populate_environment_list - Populates the environment linked list.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int populate_environment_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		adds_node_toend(&node, environ[i], 0);
	info->env = node;
	return (0);
}
