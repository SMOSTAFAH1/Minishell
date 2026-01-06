/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:46:25 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:46:25 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	has_input_redir(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == T_REDIR_IN || redirs->type == T_HEREDOC)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}

int	get_wait_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	select_pipe_input(t_cmd *cmds, int pipefd[2])
{
	close(pipefd[1]);
	if (has_input_redir(cmds->redirs)
		&& has_input_redir(cmds->next->redirs))
	{
		close(pipefd[0]);
		return (STDIN_FILENO);
	}
	return (pipefd[0]);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	i;

	if (!s)
		return (NULL);
	arr = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			arr[i++] = get_word(s, c);
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	arr[i] = NULL;
	return (arr);
}
