/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-21 13:45:23 by shashemi          #+#    #+#             */
/*   Updated: 2025-12-21 13:45:23 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new;
	char	*eq;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			new = malloc(sizeof(t_env));
			if (!new)
				return (env);
			new->key = ft_substr(envp[i], 0, eq - envp[i]);
			new->value = ft_strdup(eq + 1);
			new->next = env;
			env = new;
		}
		i++;
	}
	return (env);
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = *env;
	*env = new;
}

void	unset_env_value(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

char	**env_to_array(t_env *env)
{
	char	**arr;
	int		i;
	int		count;
	t_env	*tmp;
	char	*str;

	count = 0;
	tmp = env;
	while (tmp && ++count)
		tmp = tmp->next;
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = ft_strjoin(env->key, "=");
		str = arr[i];
		arr[i] = ft_strjoin(str, env->value);
		free(str);
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
