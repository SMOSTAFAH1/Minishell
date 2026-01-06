/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:44:20 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:44:20 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	idx;
	int	c;

	if (!str)
		return (0);
	c = str[0];
	if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) && c != '_')
		return (0);
	idx = 1;
	while (str[idx] && str[idx] != '=')
	{
		c = str[idx];
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
				|| (c >= '0' && c <= '9')) && c != '_')
			return (0);
		idx++;
	}
	return (1);
}

static void	assign_key(char *arg, t_env **env)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return ;
	key = ft_substr(arg, 0, eq - arg);
	if (key)
	{
		set_env_value(env, key, eq + 1);
		free(key);
	}
}

int	builtin_export(char **args, t_env **env)
{
	int	idx;

	if (!args[1])
		return (builtin_env(*env));
	idx = 1;
	while (args[idx])
	{
		if (!is_valid_identifier(args[idx]))
		{
			ft_putendl_fd("minishell: export: not a valid identifier", 2);
			return (1);
		}
		assign_key(args[idx], env);
		idx++;
	}
	return (0);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

int	get_redir_type(char *input, int *i)
{
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (T_HEREDOC);
		}
		return (T_REDIR_IN);
	}
	(*i)++;
	if (input[*i] == '>')
	{
		(*i)++;
		return (T_REDIR_APPEND);
	}
	return (T_REDIR_OUT);
}
