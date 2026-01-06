/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:49:57 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:49:57 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	add_redir(t_cmd *cmd, int type, char *file)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = type;
	redir->file = file;
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

int	parse_redir(t_token **token, t_cmd *cmd)
{
	int		type;
	t_token	*tmp;
	char	*file;

	type = (*token)->type;
	tmp = *token;
	*token = (*token)->next;
	free(tmp->value);
	free(tmp);
	if (!*token || (*token)->type != T_WORD)
		return (-1);
	file = ft_strdup((*token)->value);
	if (!file)
		return (-1);
	add_redir(cmd, type, file);
	tmp = *token;
	*token = (*token)->next;
	free(tmp->value);
	free(tmp);
	return (0);
}

void	skip_pipe_token(t_token **tokens)
{
	t_token	*tmp;

	if (*tokens && (*tokens)->type == T_PIPE)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_redir	*rtmp;
	int		i;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		i = 0;
		while (tmp->args && tmp->args[i])
			free(tmp->args[i++]);
		free(tmp->args);
		while (tmp->redirs)
		{
			rtmp = tmp->redirs;
			tmp->redirs = tmp->redirs->next;
			free(rtmp->file);
			free(rtmp);
		}
		free(tmp);
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	return (joined);
}
