/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:46:53 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:46:53 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	free_paths(char **paths, int i)
{
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*check_path(char **paths, char *cmd)
{
	char	*tmp;
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
		{
			free_paths(paths, i);
			return (path);
		}
		free(path);
		free(paths[i++]);
	}
	free(paths);
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	return (check_path(paths, cmd));
}

static void	exec_child(t_cmd *cmd, t_env **env, char *path)
{
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	envp = env_to_array(*env);
	if (!envp)
		exit(1);
	execve(path, cmd->args, envp);
	if (errno == ENOEXEC)
		exec_with_sh(cmd, envp, path);
	print_exec_error(cmd->args[0], errno);
	exit(get_exec_status(errno));
}

int	exec_external(t_cmd *cmd, t_env **env, char *path)
{
	pid_t	pid;
	int		status;
	int		check;

	check = validate_path(path, cmd->args[0]);
	if (check != 0)
		return (check);
	pid = fork();
	if (pid == 0)
		exec_child(cmd, env, path);
	free(path);
	waitpid(pid, &status, 0);
	return (get_wait_status(status));
}
