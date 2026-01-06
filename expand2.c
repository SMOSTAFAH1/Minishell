/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:47:18 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:47:18 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	char	*name;
	char	c;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	c = str[*i];
	while (c && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9') || c == '_'))
	{
		(*i)++;
		c = str[*i];
	}
	name = ft_substr(str, start, *i - start);
	return (name);
}

char	*expand_in_str(char *str, t_shell *shell, int *i)
{
	char	*var_name;
	char	*value;
	char	*result;

	(*i)++;
	var_name = get_var_name(str, i);
	if (ft_strcmp(var_name, "?") == 0)
		value = ft_itoa(shell->exit_status);
	else
		value = get_env_value(shell->env, var_name);
	if (!value)
		result = ft_strdup("");
	else
		result = ft_strdup(value);
	if (ft_strcmp(var_name, "?") == 0)
		free(value);
	free(var_name);
	return (result);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

char	*get_next_part(char *str, t_shell *shell, int *i)
{
	int		start;
	char	*tmp;
	char	next;

	start = *i;
	if (str[*i] == '$')
	{
		next = str[*i + 1];
		if (next && (next == '?' || ((next >= 'a' && next <= 'z')
					|| (next >= 'A' && next <= 'Z')
					|| (next >= '0' && next <= '9'))
				|| next == '_'))
			return (expand_in_str(str, shell, i));
		(*i)++;
		return (ft_strdup("$"));
	}
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$')
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	return (tmp);
}

char	*expand_var_loop(char *str, t_shell *shell)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			tmp = process_quotes(str, shell, &i, str[i]);
		else
			tmp = get_next_part(str, shell, &i);
		result = join_and_free(result, tmp);
	}
	return (result);
}
