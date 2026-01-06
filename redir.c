/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:50:09 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:50:09 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	open_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	open_redir_out(char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_redir *redirs, t_shell *shell)
{
	while (redirs)
	{
		if (redirs->type == T_REDIR_IN)
		{
			if (open_redir_in(redirs->file) < 0)
				return (-1);
		}
		else if (redirs->type == T_REDIR_OUT)
		{
			if (open_redir_out(redirs->file, 0) < 0)
				return (-1);
		}
		else if (redirs->type == T_REDIR_APPEND)
		{
			if (open_redir_out(redirs->file, 1) < 0)
				return (-1);
		}
		else if (redirs->type == T_HEREDOC)
		{
			if (handle_heredoc(redirs->file, shell) < 0)
				return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}

static int	read_heredoc_line(char *delim, int fd, t_shell *shell)
{
	char	*line;

	line = readline("> ");
	if (!line && g_signal == SIGINT)
	{
		shell->in_heredoc = 0;
		close(fd);
		exit(130);
	}
	if (!line || ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

int	handle_heredoc(char *delim, t_shell *shell)
{
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	shell->in_heredoc = 1;
	while (read_heredoc_line(delim, pipefd[1], shell))
		;
	shell->in_heredoc = 0;
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}
