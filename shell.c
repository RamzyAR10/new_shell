#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
char *handle_exec_file(char *first_arg1, char *path);
int path_execution(char *path, char **args, char *envp[]);
char **dup_2D(char *list[])
{
	int rows = 0, i = 0;
	char **list_dup;

	while (*(list + rows) != NULL)
	{
		rows++;
	}

	list_dup = (char **)malloc(sizeof(char *) * (rows + 1));

	if (list_dup == NULL)
	{
		free(list_dup);
	}

	for (i = 0; list[i]; i++)
	{
		list_dup[i] = strdup(list[i]);
	}
	list_dup[i] = NULL;
	return (list_dup);
}
int _strcmp(char *string1, char *string2)
{
	int i = 0;

	while (string1[i] != '\0' && string2[i] != '\0')
	{
		if (string1[i] != string2[i])
		{
			return (string1[i] - string2[i]);
		}
		i++;
	}
	return (string1[i] - string2[i]);
}
void remove_newline(char *tmp)
{
	int i = 0;

	while (tmp[i])
	{
		if (tmp[i] == '\n')
		{
			tmp[i] = '\0';
		}
		i++;
	}
}
/*important*/
char *handle_PATH(char *envp[])
{
	int i = 0;
	char *PATH_key = "PATH=";
	char *PATH_value = NULL;
	char envp_key[BUFSIZ];
	size_t l_path = strlen(PATH_key);

	while (envp[i])
	{
		memcpy(envp_key, envp[i], l_path); /*for compar key with key*/
		envp_key[l_path] = '\0';		   /*to store only key*/

		if (_strcmp(envp_key, PATH_key) == 0)
		{
			PATH_value = envp[i] + l_path; /*pointer arthmatic for get value*/
			return (PATH_value);
		}
		i++;
	}
	return (NULL);
}
char *check_exec_file(char *cmd, char *path)
{
	char *first_arg1 = NULL;
	char *cmd_cpy = strdup(cmd);

	first_arg1 = strtok(cmd_cpy, " ");

	if (strchr(first_arg1, '/') && access(first_arg1, X_OK) == 0)
	{
		printf(">>1 %s\n", first_arg1);
		return (first_arg1);
	}
	else
	{
		return (handle_exec_file(first_arg1, path));
	}
}
char *handle_exec_file(char *first_arg1, char *path)
{
	char *Path_cpy = strdup(path);

	char *current_Path = strtok(Path_cpy, ":");

	while (current_Path && *path)
	{
		size_t len = strlen(first_arg1) + strlen(current_Path) + 2;
		char *first_arg2 = malloc(sizeof(char) * len);
		int i = 0, x = 0;

		while (current_Path[i])
		{
			first_arg2[i] = current_Path[i];
			i++;
		}
		first_arg2[i] = '/';
		i++; /*why ask ashraf*/
		while (first_arg1[x])
		{
			first_arg2[i] = first_arg1[x];
			i++, x++;
		}
		first_arg2[i] = '\0';

		if (access(first_arg2, X_OK) == 0)
		{
			printf(">>2 %s\n", first_arg2);

			return (first_arg2);
		}

		current_Path = strtok(NULL, ":");
	}
	return (NULL);
}
char **handle_argument(char *path, char *cmd)
{
	int i = 0, x = 1;
	char *args[BUFSIZ];
	char *argin[BUFSIZ];
	char **result;

	char *cmd_cpy = strdup(cmd);
	char *chank = strtok(cmd_cpy, " ");

	while (chank && cmd)
	{
		argin[i] = chank;
		i++;
		chank = strtok(NULL, " ");
	}

	args[0] = path;
	while (argin[x])
	{
		args[x] = argin[x];
		x++;
	}
	args[x] = NULL;

	result = dup_2D(args);

	return (result);
}
int exec_process(char *exec_path, char **args, char **envp)
{
	int pid = fork();

	if (pid < 0)
	{
		perror("error in exec process");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		int status = path_execution(exec_path, args, envp);

		if (status == -1)
		{
			printf("exError\n");
		}
		return (status);
	}
	else
	{
		int state;
		wait(&state);
		return (state);
	}
}
int path_execution(char *path, char **args, char *envp[])
{
	execve(path, args, envp);

	return (2);
}
int main(int argc, char *argv[], char *envp[])
{
	argv[argc - 1] = argv[argc - 1];
	char *exec_path;
	char **args;
	char cmd_from_terminal[BUFSIZ];

	char *Path = NULL;

	int status;

	while (1)
	{
		/*read from user input */
		write(1, "-->", 4);
		read(0, cmd_from_terminal, BUFSIZ);

		/*remove new line from my code*/
		remove_newline(cmd_from_terminal);
		Path = handle_PATH(envp);

		/*printf(">>path %s\n", Path);*/
		/*take the exec file*/
		exec_path = check_exec_file(cmd_from_terminal, Path);

		if (exec_path == NULL)
		{
			printf("not found \n");
		}
		args = handle_argument(exec_path, cmd_from_terminal);

		status = exec_process(exec_path, args, envp);
	}
	return (status);
}
