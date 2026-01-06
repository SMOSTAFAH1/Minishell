/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:49:44 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:49:44 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * MAX_ARGS);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static t_cmd	*parse_simple_cmd(t_token **tokens)
{
	t_cmd	*cmd;
	int		i;
	t_token	*tmp;

	cmd = create_cmd();
	i = 0;
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_WORD)
		{
			cmd->args[i++] = ft_strdup((*tokens)->value);
			tmp = *tokens;
			*tokens = (*tokens)->next;
			free(tmp->value);
			free(tmp);
		}
		else if (parse_redir(tokens, cmd) < 0)
			return (NULL);
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*cmd;
	t_cmd	*last;

	(void)shell;
	cmds = NULL;
	last = NULL;
	while (tokens)
	{
		cmd = parse_simple_cmd(&tokens);
		if (!cmd)
			return (NULL);
		if (!cmds)
			cmds = cmd;
		else
			last->next = cmd;
		last = cmd;
		skip_pipe_token(&tokens);
	}
	return (cmds);
}

static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(t_shell *shell)
{
	(void)shell;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
