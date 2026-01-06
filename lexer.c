/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:47:40 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:47:40 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

static void	add_token(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	skip_quotes(char *input, int *i, char quote)
{
	int	start;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	return (*i - start);
}

static char	*extract_word(char *input, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
			skip_quotes(input, i, input[*i]);
		else
			(*i)++;
	}
	word = ft_substr(input, start, *i - start);
	return (word);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|')
		{
			add_token(&tokens, create_token(T_PIPE, ft_strdup("|")));
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
			add_token(&tokens, create_token(get_redir_type(input, &i),
					ft_strdup("")));
		else
			add_token(&tokens, create_token(T_WORD, extract_word(input, &i)));
	}
	return (tokens);
}
