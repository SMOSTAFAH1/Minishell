/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:46:36 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:46:36 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
		return (exec_builtin(cmd, shell));
	path = find_cmd_path(cmd->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	return (exec_external(cmd, &shell->env, path));
}

static int	exec_last_cmd(t_cmd *cmd, t_shell *shell, int in_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (handle_redirections(cmd->redirs, shell) < 0)
			exit(1);
		exit(exec_cmd(cmd, shell));
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	waitpid(pid, &status, 0);
	return (get_wait_status(status));
}

static int	exec_pipe_child(t_cmd *cmds, t_shell *shell, int in_fd,
		int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (handle_redirections(cmds->redirs, shell) < 0)
			exit(1);
		exit(exec_cmd(cmds, shell));
	}
	return (pid);
}

static int	exec_pipeline(t_cmd *cmds, t_shell *shell, int in_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int		child_status;
	int		next_fd;

	if (pipe(pipefd) < 0)
		return (1);
	pid = exec_pipe_child(cmds, shell, in_fd, pipefd);
	if (pid < 0)
		return (1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	next_fd = select_pipe_input(cmds, pipefd);
	if (cmds->next->next)
		status = exec_pipeline(cmds->next, shell, next_fd);
	else
		status = exec_last_cmd(cmds->next, shell, next_fd);
	waitpid(pid, &child_status, 0);
	return (status);
}

int	executor(t_cmd *cmds, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	signal(SIGINT, SIG_IGN);
	if (cmds->next)
	{
		status = exec_pipeline(cmds, shell, STDIN_FILENO);
		setup_signals(shell);
		return (status);
	}
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmds->redirs, shell) < 0)
		status = 1;
	else
		status = exec_cmd(cmds, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	setup_signals(shell);
	return (status);
}
