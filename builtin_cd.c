/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:43:00 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:43:00 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	print_cd_error(char *path, char *msg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (path)
	{
		ft_putstr_fd(path, 2);
		ft_putendl_fd(msg, 2);
	}
	else
		ft_putendl_fd(msg, 2);
	return (1);
}

static char	*get_cd_path(char **args, t_env **env)
{
	char	*path;

	if (args[2])
		return (NULL);
	if (args[1] && args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value(*env, "OLDPWD");
		if (!path)
			return (NULL);
		return (path);
	}
	path = args[1];
	if (!path)
	{
		path = get_env_value(*env, "HOME");
		if (!path)
			return (NULL);
	}
	return (path);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*path;
	char	cwd[MAX_PATH];
	char	*oldpwd;

	path = get_cd_path(args, env);
	if (!path)
	{
		if (args[2])
			return (print_cd_error(NULL, "too many arguments"));
		return (print_cd_error(NULL, "HOME not set"));
	}
	oldpwd = get_env_value(*env, "PWD");
	if (chdir(path) < 0)
		return (print_cd_error(path, ": No such file or directory"));
	if (oldpwd)
		set_env_value(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_value(env, "PWD", cwd);
		if (args[1] && args[1][0] == '-' && args[1][1] == '\0')
			ft_putendl_fd(cwd, 1);
	}
	return (0);
}

int	builtin_unset(char **args, t_env **env)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_env_value(env, args[i]);
		i++;
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd->args, &shell->env));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, &shell->env));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd->args, &shell->env));
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(shell->env));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd->args, shell->exit_status));
	return (0);
}
