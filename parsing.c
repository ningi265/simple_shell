#include "shell.h"

/**
 * is_command - Determines if a file is an executable command
 * @info: The info struct
 * @path: Path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_command(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * duplicates_characters - Duplicates characters
 * @pathstr: The PATH string
 * @start: Starting index
 * @stop: Stopping index
 *
 * Return: Pointer to new buffer
 */
char *duplicates_characters(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * finds_path - Finds this cmd in the PATH string
 * @info: The info struct
 * @pathstr: The PATH string
 * @cmd: The cmd to find
 *
 * Return: Full path of cmd if found or NULL
 */
char *finds_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_string_length(cmd) > 2) && starts_with_haystack(cmd, "./"))
	{
		if (is_command(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = duplicates_characters(pathstr, curr_pos, i);
			if (!*path)
				_string_concatenate(path, cmd);
			else
			{
				_string_concatenate(path, "/");
				_string_concatenate(path, cmd);
			}
			if (is_command(info, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
