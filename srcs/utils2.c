/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 15:29:27 by malmeida          #+#    #+#             */
/*   Updated: 2022/01/07 14:09:19 by malmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_one_bi(int x, int index)
{
	char	*str;

	str = g_mini.cmd[x].command[0];
	if (!ft_strcmp(str, "cd"))
		return (bi_cd(x, g_mini.cmd[x].command[1], index));
	if (!ft_strcmp(str, "pwd"))
		return (bi_pwd());
	if (!ft_strcmp(str, "env"))
		return (bi_env());
	if (!ft_strcmp(str, "echo"))
		return (bi_echo(x, g_mini.cmd[x].command));
	if (!ft_strcmp(str, "export"))
		return (bi_export(g_mini.cmd[x].command, index));
	if (!ft_strcmp(str, "unset"))
		return (bi_unset(g_mini.cmd[x].command));
	return (-1);
}

int	is_builtin(int x)
{
	char	*str;

	str = g_mini.cmd[x].command[0];
	if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "export") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "cd"))
	{
		if ((!ft_strcmp(str, "export") && g_mini.cmd[x].command[1] != NULL)
			|| (!ft_strcmp(str, "unset") && g_mini.cmd[x].command[1] != NULL)
			|| !ft_strcmp(str, "cd"))
			return (2);
		return (1);
	}
	return (0);
}

int	get_bin_path(void)
{
	int	i;

	i = -1;
	while (g_mini.env[++i] != NULL)
		if (g_mini.env[i][0] == 'P' && g_mini.env[i][1] == 'A')
			break ;
	if (g_mini.env[i] == NULL && is_builtin(0) == 0)
		return (-1);
	else if (g_mini.env[i] == NULL && is_builtin(0) != 0)
		return (0);
	g_mini.bin_paths = ft_split(g_mini.env[i] + 5, ':');
	return (0);
}

int	cat_fix(int c)
{
	while (ft_strcmp(g_mini.cmd[c].command[0], "cat") == 0)
	{
		g_mini.pipes--;
		g_mini.num_cmds--;
		c++;
	}
	return (c);
}

void	exec_cat(int c, int nmr)
{
	(void)c;
	while (nmr > 0)
	{
		readline(NULL);
		nmr--;
	}
}
