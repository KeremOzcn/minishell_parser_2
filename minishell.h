/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kozcan <kozcan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:22:52 by kozcan            #+#    #+#             */
/*   Updated: 2025/07/01 17:22:55 by kozcan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h> 
# include <unistd.h>
# include "./libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/stat.h>
# include <termios.h>
# include "readline/readline.h"
# include "readline/history.h"
# include <signal.h>

typedef enum e_tokens
{
	PIPE = 1,
	RED_OUT,
	APPEND,
	RED_IN,
	HEREDOC
}		t_tokens;

typedef struct s_proc
{
	int		fd[2];
	pid_t	pid;
}		t_proc;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				is_quote;
	int				i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}		t_lexer;

typedef struct s_global
{
	t_lexer					*head;
	struct s_simple_cmds	*p_head;
	char					**env;
	char					**export;
}		t_global;

typedef struct s_simple_cmds
{
	char					**str;
	int						(*builtin)(t_global *, struct s_simple_cmds *);
	int						num_redirections;
	int						in_her;
	char					*hd_file_name;
	t_proc					prcs;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}		t_simple_cmds;

typedef struct s_globals
{
	int		a;
	int		error_num;
	int		in_her;
	int		in_red;
	int		test1;
	int		out_red;
	int		test2;
	char	**env;
}		t_globals;

extern t_globals	g_global;

char	**ft_setenv(char **ev);
void	ft_init_lexer(t_global *mini, char *line);
int		ft_quote(char *line);
void	ft_first_node(t_global *mini, char *str);
void	ft_last_node(t_global *mini, char *str);
char	**ft_split_tok(char *str);
void	ft_spt3(char ***tokens, int *i, int *tok_len, char **str);
void	ft_spt4(char ***tokens, int *i, int *tok_len, char **str);
char	*ft_strncpy(char *dest, char *src, int n);
void	free_tokens(char **tokens);
int		ft_check_error(t_global *mini);
void	ft_deallocate_lexer(t_global *mini);
void	ft_init_parser(t_global *mini);
void	ft_skipinit(t_simple_cmds **new, int flag);
void	ft_parser_lexpush_back(t_simple_cmds *temporary, char *str);
int		ft_find_token(char *str);
void	ft_delete_node(int index, t_global *mini);
void	ft_init_matrix(t_global *mini);
void	ft_deallocate_all(t_global *mini);
int		ft_strcmp(char *s1, char *s2);

#endif