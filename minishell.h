/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:49:00 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:49:00 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_ARGS 1024
# define MAX_PATH 4096

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		exit_status;
	int		in_heredoc;
}	t_shell;

extern volatile sig_atomic_t	g_signal;

size_t		count_words(char const *s, char c);
char		*get_word(char const *s, char c);
char		**ft_split(char const *s, char c);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
char		*ft_itoa(int n);
long long	ft_atoll(const char *str);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
t_token		*lexer(char *input);
void		free_tokens(t_token *tokens);
int			get_redir_type(char *input, int *i);
t_cmd		*parser(t_token *tokens, t_shell *shell);
void		free_cmds(t_cmd *cmds);
int			parse_redir(t_token **token, t_cmd *cmd);
void		skip_pipe_token(t_token **tokens);
char		*expand_in_str(char *str, t_shell *shell, int *i);
char		*process_quotes(char *str, t_shell *shell, int *i, char quote);
char		*expand_var_loop(char *str, t_shell *shell);
char		*get_next_part(char *str, t_shell *shell, int *i);
char		*join_and_free(char *s1, char *s2);
int			executor(t_cmd *cmds, t_shell *shell);
void		print_exec_error(char *cmd, int err);
int			get_exec_status(int err);
void		exec_with_sh(t_cmd *cmd, char **envp, char *path);
int			report_path_error(char *cmd, char *path);
int			validate_path(char *path, char *cmd);
int			has_input_redir(t_redir *redirs);
int			get_wait_status(int status);
int			select_pipe_input(t_cmd *cmds, int pipefd[2]);
void		setup_signals(t_shell *shell);
t_env		*init_env(char **envp);
char		*get_env_value(t_env *env, char *key);
void		set_env_value(t_env **env, char *key, char *value);
void		unset_env_value(t_env **env, char *key);
char		**env_to_array(t_env *env);
void		free_env(t_env *env);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_env **env);
int			builtin_pwd(void);
int			builtin_export(char **args, t_env **env);
int			builtin_unset(char **args, t_env **env);
int			builtin_env(t_env *env);
int			builtin_exit(char **args, int exit_status);
int			is_builtin(char *cmd);
int			exec_builtin(t_cmd *cmd, t_shell *shell);
int			handle_redirections(t_redir *redirs, t_shell *shell);
int			handle_heredoc(char *delim, t_shell *shell);
char		*find_cmd_path(char *cmd, t_env *env);
int			exec_external(t_cmd *cmd, t_env **env, char *path);

#endif
