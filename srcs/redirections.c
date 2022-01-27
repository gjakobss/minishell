/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gjakobss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 19:28:47 by gjakobss          #+#    #+#             */
/*   Updated: 2021/12/20 19:28:51 by gjakobss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	send_output2(int fd, int index, int c)
{
	char	*buff;
	int i;
	int temp;

	i = 0;
	fd = open(g_mini.cmd[c].command[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	close(g_mini.pipefd[index][1]);
	close(g_mini.pipefd[index][0]);
	while (get_next_line(g_mini.pipefd[index - 1][0], &buff) != 0)
	{
		if (i > 0)
			write(fd, "\n", 1);
		write(fd, buff, ft_strlen(buff));
		i++;
	}
	temp = c - 1;
	while (temp >= 0)
	{
		if (ft_strcmp(g_mini.cmd[temp].command[0], "echo") == 0)
			break;
		temp--;
	}
	if (temp >= 0 && ft_strcmp(g_mini.cmd[temp].command[0], "echo") == 0)
	{
		temp++;
		write(fd, " ", 1);
 		while (temp <= c)
		{
			i = 0;
			while (g_mini.cmd[temp].command[++i])
			{
				write(fd, ft_strjoin(g_mini.cmd[temp].command[i], " "),
				ft_strlen(g_mini.cmd[temp].command[i]) + 1);
			}
			temp++;
		}
	}
	write(fd, "\n", 1);
	close(fd);
	exit(0);
}

int	send_output(int c, int index, int i)
{
	int	fd;
	int	id;
	int	temp;

	fd = 0;
	(void)i;
	temp = c;
	while (g_mini.cmd[c].op == 2)
	{
		fd = open(g_mini.cmd[c].command[0], O_CREAT | O_TRUNC, 0777);
		close(fd);
		c++;
	}
	while (temp > 1 && g_mini.cmd[temp - 2].op == 2)
	{
		fd = open(g_mini.cmd[temp - 1].command[0], O_CREAT | O_TRUNC, 0777);
		close(fd);
		temp--;
	}
	fd = 0;
	id = fork();
	if (id == 0)
		send_output2(fd, index, c);
	wait(NULL);
	close(g_mini.pipefd[index][1]);
	close(g_mini.pipefd[index][0]);
	if (g_mini.cmd[c].op == 6)
		return (-2);
	return (0);
}

int	append_output(int c, int index, int i)
{
	int		fd;
	int		id;
	char	*buff;

	id = fork();
	if (id == 0)
	{
		fd = open(g_mini.cmd[c].command[0],
				O_WRONLY | O_CREAT | O_APPEND, 0777);
		close(g_mini.pipefd[index][1]);
		close(g_mini.pipefd[index][0]);
		while (get_next_line(g_mini.pipefd[index - 1][0], &buff) != 0)
		{
			if (i == 0)
				write(fd, "\n", 1);
			write(fd, buff, ft_strlen(buff));
			i = 0;
		}
		if (ft_strcmp(g_mini.cmd[c - 1].command[0], "echo") == 0)
		{
 			write(fd, " ", 1);
 			while (g_mini.cmd[c].command[++i])
				write(fd, ft_strjoin(g_mini.cmd[c].command[i], " "),
					ft_strlen(g_mini.cmd[c].command[i]) + 1);
		}
 		write(fd, "\n", 1);
		exit(0);
	}
	wait(NULL);
	close(g_mini.pipefd[index][1]);
	close(g_mini.pipefd[index][0]);
	return (0);
}

int	send_input(int c, int index)
{
	int		i;
	char	*temp;

	(void)index;
	i = 0;
	if (!g_mini.cmd[c].command[1])
		while (g_mini.cmd[c + 1].command[i])
			i++;
	else
		return (0);
	temp = ft_strdup(g_mini.cmd[c].command[0]);
	free(g_mini.cmd[c].command[0]);
	free(g_mini.cmd[c].command);
	g_mini.cmd[c].command = malloc(sizeof(char *) * 3);
	g_mini.cmd[c].command[0] = ft_strdup(temp);
	g_mini.cmd[c].command[1] = ft_strdup(g_mini.cmd[c + 1].command[i - 1]);
	g_mini.cmd[c].command[2] = NULL;
	return (0);
}

int	wait_input(int c, int index)
{
	char	*line;
	char	*temp;

	(void)index;
	line = readline("heredoc> ");
	while (ft_strcmp(line, g_mini.cmd[c + 1].command[0]) != 0)
		line = readline("heredoc> ");
	if (g_mini.num_cmds > 2)
	{
		temp = ft_strdup(g_mini.cmd[c].command[0]);
		free(g_mini.cmd[c].command[0]);
		g_mini.cmd[c].command[0] = ft_strdup(g_mini.cmd[c + 1].command[0]);
		free(g_mini.cmd[c + 1].command[0]);
		g_mini.cmd[c + 1].command[0] = ft_strdup(temp);
	}
	return (0);
}
