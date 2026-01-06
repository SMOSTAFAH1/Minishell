/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:48:31 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:48:31 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	expand_args(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		j;
	char	*exp;

	i = -1;
	while (cmd->args[++i])
	{
		exp = expand_var_loop(cmd->args[i], shell);
		free(cmd->args[i]);
		cmd->args[i] = exp;
	}
	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '\0')
			free(cmd->args[i++]);
		else if (i != j)
			cmd->args[j++] = cmd->args[i++];
		else if (++j)
			i++;
	}
	cmd->args[j] = NULL;
}

static void	expand_redirections(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*expanded;

	redir = cmd->redirs;
	while (redir)
	{
		expanded = expand_var_loop(redir->file, shell);
		free(redir->file);
		redir->file = expanded;
		redir = redir->next;
	}
}

static int	quote_check(char *s)
{
	char	q;

	q = 0;
	while (*s)
	{
		if (!q && (*s == '"' || *s == '\''))
			q = *s;
		else if (*s == q)
			q = 0;
		s++;
	}
	return (q != 0);
}

static void	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;
	t_cmd	*tmp;

	if (!input || !*input)
		return ;
	if (quote_check(input))
		return (ft_putendl_fd("syntax error: unclosed quote", 2),
			(void)(shell->exit_status = 1));
	add_history(input);
	tokens = lexer(input);
	if (!tokens)
		return ;
	cmds = parser(tokens, shell);
	if (!cmds)
		return (free_tokens(tokens));
	tmp = cmds;
	while (tmp && (expand_args(tmp, shell), expand_redirections(tmp, shell), 1))
		tmp = tmp->next;
	shell->exit_status = executor(cmds, shell);
	free_cmds(cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	shell.env = init_env(envp);
	shell.exit_status = 0;
	shell.in_heredoc = 0;
	setup_signals(&shell);
	while (1)
	{
		input = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell.exit_status = 130;
			g_signal = 0;
		}
		if (!input)
			break ;
		process_input(input, &shell);
		free(input);
	}
	write(1, "exit\n", 5);
	free_env(shell.env);
	return (shell.exit_status);
}
