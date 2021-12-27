/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gjakobss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 19:26:14 by gjakobss          #+#    #+#             */
/*   Updated: 2021/12/27 11:45:23 by malmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <signal.h>

# include "../libft/libft.h"
# include "get_next_line.h"

# define PIPE 1
# define GREATER 2
# define SMALLER 3
# define DGREATER 4
# define DSMALLER 5
# define EOL 6
# define DQUOTES 7
# define SQUOTES 8

typedef struct s_cmds {
	char	*full_line;
	char	**command;
	int		op;
}		t_cmds;

typedef struct s_mini
{
	char	**bin_paths;
	char	**env;
	char	**exp;
	int		**pipefd;
	int		pipes;
	int		s_quotes;
	int		d_quotes;
	int		semi_col;
	int		dollar;
	t_cmds	*cmd;
	int		num_cmds;
	int		sig_int;
	int		status;
}				t_mini;

typedef struct s_expand
{
	char	*full;
	char	*before;
	char	*var;
	char	*after;
}				t_expand;

t_mini	g_mini;

/*		Built-ins		*/
int	bi_env(void);
int	bi_pwd(void);
int	bi_cd(char *str);
int	bi_echo(char **str);
int	bi_export(char **buff);
int	bi_unset(char **buff);

/*		Arg Validation	*/
int		arg_validation(char *line);
int		quotes_checker(char *line);
int		is_squote(char c);
int		is_dquote(char c);
void	operators_count(char *line);
int		check_commands(char *line);

/*		Arg Parser	*/
t_cmds	*parser(char *line);
int		is_terminator(char *line, int *j);
int		command_counter(char *line);

/*		Lexer		*/
void	quotes_skipper(char *line, int *i, int quotes);
int		assign_operator(char *line, int *i);
void	assign_line(t_cmds *cmd, char *line);
void	lexer(t_cmds *cmd, char *line);

/*		Expand Variables	*/
void	expand_variables(char **line, int start);
void	expander(t_cmds *cmd);

/*		Splitter		*/
char	**splitter(char const *s, char c);
int		is_quote(char c);

/*		Signals		*/
void	activate_signals(void);

int		exec_one2(int j, int i);
int		exec_com2(int c, int i, int com);
int		exec_last_com(int c, int index);
int		exec_com_mid(int c, int index);
void	exec_one(void);
int		exec_com_one(int c, int index);
int		divergent(int c, int index, int id);
int		one_time(int c, int index);
int		multi_exec(int c, int index, int i);
int		send_to_exec(void);
int		exec_one_bi(int x);
char	**exp_organizer(char **exp, char *str, int i, int y);
char	*exp_organizer2(char **ret, int i);
int		exec_one_bi(int x);
int		is_builtin(int x);
int		get_bin_path(void);
void	parse_args(void);
int		send_output(int c, int index, int check);
int		append_output(int c, int index, int i);
int		send_input(int c, int index);
int		wait_input(int c, int index);
#endif