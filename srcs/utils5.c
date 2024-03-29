/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:25:38 by malmeida          #+#    #+#             */
/*   Updated: 2022/02/05 17:25:41 by malmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	if_ls(t_chars *x, int c)
{
	x->i = -1;
	while (g_mini.cmd[c].command[++x->i])
		x->sub = ft_str3join(x->sub, " ", g_mini.cmd[c].command[x->i]);
	x->temp = ft_str3join(x->sub, " ", x->temp);
	free(g_mini.cmd[c].command);
	g_mini.cmd[c].command = splitter(x->temp, ' ');
}

void	quotes_skipper(char *line, int *i, int quotes)
{
	if (quotes == DQUOTES)
	{
		(*i)++;
		while (line[*i] != '"' && line[*i] != '\0')
			(*i)++;
	}
	else if (quotes == SQUOTES)
	{
		(*i)++;
		while (line[*i] != '\'' && line[*i] != '\0')
			(*i)++;
	}
}

void	close_pipes(int index)
{
	close(g_mini.pipefd[index][1]);
	close(g_mini.pipefd[index][0]);
}

void	open_close_fd(int *fd, int *c, int *temp, int x)
{
	if (x > 0 && x < 3)
	{
		*fd = open(g_mini.cmd[*c].command[0], \
		O_WRONLY | O_CREAT | O_APPEND, 0777);
		close(*fd);
	}
	if (x > 2 && x < 5)
	{
		*fd = open(g_mini.cmd[*c].command[0], O_CREAT | O_TRUNC, 0777);
		close(*fd);
	}
	if (x == 1 || x == 3)
		*c = *c + 1;
	if (x == 2 || x == 4)
		*temp = *temp - 1;
}

void	send_output3(int temp, int fd, int c)
{
	int	i;

	if (temp >= 0 && ft_strcmp(g_mini.cmd[temp].command[0], "echo") == 0)
	{
		temp++;
		write(fd, " ", 1);
		while (temp <= c)
		{
			i = 0;
			while (g_mini.cmd[temp].command[++i])
			{
				write(fd, ft_strjoin(g_mini.cmd[temp].command[i], " "), \
				ft_strlen(g_mini.cmd[temp].command[i]) + 1);
			}
			temp++;
		}
	}
}
