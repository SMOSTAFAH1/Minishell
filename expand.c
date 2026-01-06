/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:47:03 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:47:03 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_' || c == '?');
}

static int	should_expand(char *str, int i)
{
	if (!str[i + 1])
		return (0);
	if (str[i + 1] == '"' || str[i + 1] == '\'')
		return (0);
	if (is_valid_var_char(str[i + 1]))
		return (1);
	return (0);
}

static char	*get_quote_part(char *str, t_shell *shell, int *i, char quote)
{
	char	*tmp;
	int		start;

	start = *i;
	if (str[*i] == '$' && quote == '"' && should_expand(str, *i))
		tmp = expand_in_str(str, shell, i);
	else if (str[*i] == '$' && quote == '"')
	{
		(*i)++;
		tmp = ft_strdup("$");
	}
	else if (quote == '"')
	{
		while (str[*i] && str[*i] != quote && str[*i] != '$')
			(*i)++;
		tmp = ft_substr(str, start, *i - start);
	}
	else
	{
		while (str[*i] && str[*i] != quote)
			(*i)++;
		tmp = ft_substr(str, start, *i - start);
	}
	return (tmp);
}

char	*process_quotes(char *str, t_shell *shell, int *i, char quote)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		tmp = get_quote_part(str, shell, i, quote);
		result = join_and_free(result, tmp);
	}
	if (str[*i] == quote)
		(*i)++;
	return (result);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}
