/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:46:13 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:46:13 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	print_exec_error(char *cmd, int err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (err == ENOENT)
		ft_putendl_fd(": No such file or directory", 2);
	else if (err == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else if (err == ENOEXEC)
		ft_putendl_fd(": Exec format error", 2);
	else if (err == EISDIR)
		ft_putendl_fd(": Is a directory", 2);
	else
		ft_putendl_fd(": command not executable", 2);
}

int	get_exec_status(int err)
{
	if (err == ENOENT)
		return (127);
	if (err == EACCES || err == ENOEXEC || err == EISDIR || err == ENOTDIR)
		return (126);
	return (1);
}

void	exec_with_sh(t_cmd *cmd, char **envp, char *path)
{
	char	**args;
	int		count;
	int		idx;

	count = 0;
	while (cmd->args[count])
		count++;
	args = malloc(sizeof(char *) * (count + 2));
	if (!args)
		exit(1);
	args[0] = "/bin/sh";
	args[1] = path;
	idx = 1;
	while (cmd->args[idx])
	{
		args[idx + 1] = cmd->args[idx];
		idx++;
	}
	args[count + 1] = NULL;
	execve("/bin/sh", args, envp);
	print_exec_error("/bin/sh", errno);
	exit(126);
}

int	report_path_error(char *cmd, char *path)
{
	if (errno == ENOENT)
		print_exec_error(cmd, ENOENT);
	else
		print_exec_error(cmd, EACCES);
	free(path);
	if (errno == ENOENT)
		return (127);
	return (126);
}

int	validate_path(char *path, char *cmd)
{
	struct stat	info;

	if (stat(path, &info) < 0)
		return (report_path_error(cmd, path));
	if (S_ISDIR(info.st_mode))
	{
		print_exec_error(cmd, EISDIR);
		free(path);
		return (126);
	}
	return (0);
}
