/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:44:30 by lsouza-r          #+#    #+#             */
/*   Updated: 2025/01/08 19:27:29 by lsouza-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096

# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libs/libft/libft.h"
# include "../libs/ft_printf/src/ft_printf.h"
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

extern int volatile	g_signal;
//****************************************************************************//
//                               TOKENIZER                                    //
//****************************************************************************//

typedef enum e_token
{
	WORD,
	PIPE,
	REDIRECT_INPUT,
	REDIRECT_HEREDOC,
	REDIRECT_OUTPUT,
	REDIRECT_OUTPUT_APPEND,
	COMMAND
}	t_token_enum;

/*
* @brief s_token
*
*/
typedef struct s_token
{
	int		type;
	char	*lexeme;
}	t_token;

typedef struct s_token_list
{
	int					pos;
	t_token				token;
	struct s_token_list	*next;
	struct s_token_list	*prev;
}	t_list;

typedef struct s_tkn_data
{
	int		str_len;
	int		lex_len;
	char	*lexema;
	int		state;
	int		tkn_type;
}	t_tkn_data;

typedef struct s_redir
{
	int				rd_type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_tree
{
	int				tkn_type;
	t_list			*sub_list;
	t_redir			*redir;
	int				fd[2];
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
}	t_tree;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_builtin
{
	char	**argv;
	char	*command;
	int		status_builtin;
}	t_builtin;

typedef struct s_minishell
{
	t_list		*token_list;
	t_tree		*tree;
	t_envp		*envp_list;
	t_builtin	builtin;
	t_lst		*pid;
	t_lst		*fd_list;
	char		**envp;
	char		**path;
	char		*prompt;
	int			status;
	int			fd_stdin;
	int			fd_stdout;
}	t_minishell;

typedef struct s_execve
{
	char	**args;
	char	*cmd;
}	t_execve;

// main.c
void	read_prompt(t_minishell *shell);
void	set_prompt(t_minishell *shell);
void	handle_shell_execution(t_minishell *shell);
void	free_prompt(t_minishell *shell);

// lexing.c
t_list	*tokenizer(char *str);
void	init_token_data(t_tkn_data *tkn_data, char *str);
int		is_final_state(t_tkn_data tkn_data);
void	put_token_on_list(t_tkn_data *tkn_data, char *str,
			t_list **token_list, int i);
int		get_token_type(int state);
int		token_get_next_state(int state, char c);
int		token_get_state_30(char c);
int		token_get_state_40(char c);
void	state_requires_backtrack(t_tkn_data *tkn_data, int	*i);
int		token_get_state_1(char c);
int		ft_is_space(char c);
int		is_metachar(char c);
int		token_get_state_50(char c);
int		token_get_state_51(char c);
int		token_get_state_52(char c);
void	token_clear_list(t_list **token_list);

//utils.c
void	ft_strcpy(char *prompt, char *copy);
void	token_add_to_list(t_list **token_list, char *lexeme, int token_type);
void	free_list(t_list **t_tree);
void	*ft_free_split(char **split);
void	init_shell(t_minishell *shell);
t_redir	*ft_lst_new(char *file, int rd_type);
void	ft_lst_add_back(t_redir **lst, t_redir *new);
t_redir	*ft_lst_last(t_redir *lst);
void	free_pid_list(t_lst **pid);
void	free_redir(t_redir **redir_list);
void	free_minishell(t_minishell *shell);

//parsing-tree
t_tree	*build_tree(t_list	*tkn_list);
t_list	*get_last_token(t_list	*tkn_list);
t_list	*hunt_last_pipe(t_list	*tkn_list);
t_tree	*build_root(t_list	*tkn_list);
void	build_branch(t_list *tkn_list, t_tree *pivot);
void	*free_tree(t_tree **tree);

//validation.c
int		valid_redirect(t_list *list, t_minishell *shell);
int		valid_pipe(t_list *token_list, t_minishell *shell);
int		valid_list(t_list *list, t_minishell *shell);
int		check_syntax_error(t_list *aux, t_minishell *shell);

//builtin
int		pwd(void);
int		echo(char **arg);
int		change_directory(t_envp **env_list, char **path);
void	update_pwd(t_envp **env_list, char *old_pwd, char *pwd);
int		check_path(char *new_path, char **path);
int		check_num_path(char **path);

//exec_builtin.c
int		aux_exec_builting(char *command, char **argv, t_minishell *shell);
int		is_builtin(t_tree *tree);
int		execute_builtin(t_minishell *shell, t_tree *tree);
void	get_args_builtin(t_list *sub_list, t_builtin *builtin);
void	init_builtin(t_builtin *builtin);
void	clear_args(char **args);

//env_list.c
t_envp	*env_create_node(void);
void	add_node_to_list(t_envp **head, t_envp *node);
t_envp	*node_from_environ(char *environ);
t_envp	*create_env_list(char **environ);
void	print_env_list(t_envp *env_list);
int		env(t_envp *env_list);
char	*envp_str(t_envp *env_list);
char	**list_to_str(t_envp *env_list);
int		count_nodes(t_envp *env_list);
void	free_env_list(t_envp *env_list);
void	free_envp_str(char	**envp);

//export.c
int		export_new_var(char **new_var, t_envp **env_list);
char	**new_var_split(char *arg);
char	*ft_getenv(char *arg, t_envp *env_list);
void	change_env_value(char *key, char *value, t_envp **env_list);
void	order_env_list(t_envp **env_list);
int		export(char **arg, t_envp **env_list);
int		check_key_name(char *key, char *origin);
int		is_equal(char *arg, char *origin);
int		key_exist(char *arg, t_envp *env_list);
int		export_no_args(t_envp **env_list);
int		insert_var(char *arg, t_envp **env_list);
char	**get_key_value(char *arg, char **dest);
t_envp	*env_list_copy(t_envp *env_list);

//unset.c
int		unset(char **arg, t_envp **env_list);
int		unset_no_args(void);
void	remove_node_from_list(char *arg, t_envp **env_list);

//exit.c
int		ft_exit(t_minishell *shell, char **arg);
int		check_exit_arg(char **arg);
int		check_too_many_args(char **arg);
int		is_numeric_arg(char *arg);
int		valide_arg_length(char *arg);

//executor.c
void	get_path(t_minishell *shell);
void	get_args(t_list *sub_list, t_execve *exec);
void	executor(t_tree	*tree, t_minishell *shell);
void	wait_pid(t_minishell *shell);
void	close_fd(t_minishell *shell);

//exec_cmd.c
void	exec_cmd(t_tree	*tree, t_minishell *shell);
char	*find_full_path(t_execve *exec, t_minishell *shell);
void	handle_command_not_found(t_execve *exec, t_minishell *shell);
void	check_executable(char *full_path, t_execve *exec, t_minishell *shell);
void	execute_command(char *full_path, t_execve *exec, t_minishell *shell);
void	free_error_exec(t_execve *exec, t_minishell *shell, char *full_path);
int		is_a_dir(char *path);
void	free_execve(t_execve *exec);

//signal.c
void	init_signals(void);
void	sig_handler_sigint(int signal);
void	signals_for_command(void);
void	sig_handler_execute(int signal);
void	sig_handler_heredoc(int signal);

//utils.c
void	*free_split(char **str);
void	add_status_signal(int s, t_minishell *shell);
int		ft_is_space_str(char *str);

//expander
void	expander(t_list *sub_list, t_minishell *shell);
char	*check_lexeme(char *str, t_minishell *shell, int is_delimit);
int		expander_var(char *str, int i, t_minishell *shell, char **result);
void	handle_variable(char *var, t_minishell *shell, char **result);
void	expander_word(char c, char **result);
void	handle_quotes(char c, int *quotes, char **result);
char	**handle_state(char *str, t_minishell *shell, char **result);
void	expander_heredoc(char **line, t_minishell *shell);

//heredoc.c
void	hunt_heredoc(t_list *tkn_list, t_minishell *shell);
void	create_heredoc(t_list *delimiter, int tag, t_minishell *shell);
char	*create_file_path(int tag);
void	process_heredoc_input(t_list *delimiter, t_minishell *shell,
			int fd, char *file_path);

//handle_redir.c
int		handle_output_append(t_redir *redir, char *file, t_minishell *shell);
int		handle_input_heredoc(t_redir *redir, char *file, t_minishell *shell);
int		handle_redir(t_tree	*tree, t_minishell *shell);

//handle_pipe.c
void	execute_left(t_tree *tree, t_minishell *shell);
void	execute_right(t_tree *tree, t_minishell *shell);
int		handle_pipe(t_tree *tree, t_minishell *shell, int left);

//exec_single_cmd.c
void	exec_single_cmd(t_tree *tree, t_minishell *shell);
void	exec_single_builtin(t_tree *tree, t_minishell *shell);
void	execute_single_cmd(t_tree *tree, t_minishell *shell);

//hunt_redir.c
t_redir	*hunt_redir(t_list	**tkn_list);
void	redir_removal(t_list **node, t_list	**tkn_list);

#endif
