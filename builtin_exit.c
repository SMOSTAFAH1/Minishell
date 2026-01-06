/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:43:58 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:43:58 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	is_numeric_arg(char *str)
{
	int	idx;

	idx = 0;
	if (str[idx] == '+' || str[idx] == '-')
		idx++;
	if (!str[idx])
		return (0);
	while (str[idx])
	{
		if (!(str[idx] >= '0' && str[idx] <= '9'))
			return (0);
		idx++;
	}
	return (1);
}

static void	print_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

int	builtin_exit(char **args, int exit_status)
{
	int	exit_code;

	exit_code = exit_status;
	if (args[1])
	{
		if (!is_numeric_arg(args[1]))
		{
			print_numeric_error(args[1]);
			exit(2);
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		exit_code = (int)ft_atoll(args[1]);
	}
	exit(exit_code);
}

size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

char	*get_word(char const *s, char c)
{
	size_t	len;
	char	*word;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, s, len);
	word[len] = '\0';
	return (word);
}
