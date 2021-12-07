#include "minishell.h"

void	exec_one(void)
{
	int	i;
	int	j;
	int	id;

	i = 0;
	j = 0;
	while (g_mini.bin_paths[i] != NULL)
	{
		j = access(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd->command[0]), F_OK);
		if (j == 0)
			break ;
		i++;
	}
	if (j == -1)
		printf("bbshell: command not found: %s\n", g_mini.cmd->command[0]);
	id = fork();
	if (id == 0 && j == 0)
		execve(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd->command[0]), g_mini.cmd->command, g_mini.env);
	else
		wait(NULL);
}

int	exec_com_one(int c, int index)
{
	int	i;
	int	j;
	int	id;

	i = 0;
	id = fork();
	if (id == 0)
	{
		while (g_mini.bin_paths[i] != NULL)
		{
			j = access(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), F_OK);
			if (j == 0)
				break ;
			i++;
		}
		if (j == -1 && is_builtin(c) == 0)
		{
			printf("bbshell: command not found: %s\n", g_mini.cmd[c].command[0]);
			return (-1);
		}
		close(g_mini.pipefd[index][0]);
		dup2(g_mini.pipefd[index][1], 1);
		if (is_builtin(c) == 1)
		{
			exec_one_bi(c);
			exit(0);
		}
		else
			execve(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), g_mini.cmd[c].command, g_mini.env);
	}
	wait(NULL);
	close (g_mini.pipefd[index][1]);
	return (0);
}

int	exec_com_mid(int c, int index)
{
	int	id;
	int	i;
	int	j;

	i = 0;
	id = fork();
	if (id == 0)
	{
		while (g_mini.bin_paths[i] != NULL)
		{
			j = access(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), F_OK);
			if (j == 0)
				break ;
			i++;
		}
		if (j == -1 && is_builtin(0) == 0)
		{
			printf("bbshell: command not found: %s\n", g_mini.cmd[c].command[0]);
			return (-1);
		}
		close(g_mini.pipefd[index][0]);
		dup2(g_mini.pipefd[index - 1][0], 0);
		dup2(g_mini.pipefd[index][1], 1);
		if (is_builtin(c) == 1)
		{
			exec_one_bi(c);
			exit(0);
		}
		else
			execve(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), g_mini.cmd[c].command, g_mini.env);
	}
	wait(NULL);
	close(g_mini.pipefd[index][1]);
	return (index);
}

int	exec_last_com(int c, int index)
{
	int	id;
	int	i;
	int	j;

	i = 0;
	id = fork();
	if (id == 0)
	{
		i = 0;
		while (g_mini.bin_paths[i] != NULL)
		{
			j = access(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), F_OK);
			if (j == 0)
				break ;
			i++;
		}
		if (j == -1 && is_builtin(0) == 0)
		{
			printf("bbshell: command not found: %s\n", g_mini.cmd[c].command[0]);
			return (-1);
		}
		close(g_mini.pipefd[index][1]);
		close(g_mini.pipefd[index][0]);
		dup2(g_mini.pipefd[index - 1][0], 0);
		if (is_builtin(c) == 1)
		{
			exec_one_bi(c);
			exit(0);
		}
		else
			execve(ft_str3join(g_mini.bin_paths[i], "/", g_mini.cmd[c].command[0]), g_mini.cmd[c].command, g_mini.env);
	}
	else
	{
		wait(NULL);
		close(g_mini.pipefd[index][1]);
		close(g_mini.pipefd[index][0]);
	}
	return (0);
}

int	send_to_exec(void)
{
	int	i;
	int	c;
	int	index;

	i = 0;
	c = 0;
	index = 0;
	if (g_mini.pipes == 0 && is_builtin(0) == 1)
		exec_one_bi(0);
	else if (g_mini.pipes == 0 && is_builtin(0) == 0)
		exec_one();
	else
	{
		g_mini.pipefd = malloc(sizeof(int *) * g_mini.pipes);
		while (i < g_mini.pipes)
		{
			g_mini.pipefd[i] = malloc(sizeof(int) * 2);
			pipe(g_mini.pipefd[i++]);
		}
		if (exec_com_one(c, index) == -1)
			return (-1);
		index++;
		c++;
		g_mini.pipes /= 2;
		while (--g_mini.pipes > 0)
		{
			if (exec_com_mid(c, index) == -1)
				return (-1);
			c++;
			index++;
		}
		return (exec_last_com(c, index));
	}
	return (0);
}