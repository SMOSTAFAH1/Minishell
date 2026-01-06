/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:44:39 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:44:39 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	echo_write(const char *str, int *reported)
{
	size_t		len;
	ssize_t		written;
	const char	*ptr;

	if (!str)
		return (0);
	len = ft_strlen(str);
	ptr = str;
	while (len > 0)
	{
		written = write(1, ptr, len);
		if (written < 0)
		{
			if (errno == EPIPE && !*reported)
				*reported = 1;
			return (-1);
		}
		ptr += written;
		len -= written;
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	newline;
	int	i;
	int	reported;

	newline = 1;
	reported = 0;
	i = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		if (echo_write(args[i], &reported) < 0)
			return (0);
		if (args[i + 1] && echo_write(" ", &reported) < 0)
			return (0);
		i++;
	}
	if (newline && echo_write("\n", &reported) < 0)
		return (0);
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[MAX_PATH];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	return (1);
}

int	builtin_env(t_env *env)
{
	while (env)
	{
		ft_putstr_fd(env->key, 1);
		write(1, "=", 1);
		ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
