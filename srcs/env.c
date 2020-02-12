/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 16:45:33 by cclaude           #+#    #+#             */
/*   Updated: 2020/02/12 18:42:30 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_env(char **env, const char *var)
{
	int		i;
	char	*path;

	i = 0;
	while (env[i] && ft_strncmp(env[i], var, ft_strlen(var)) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	path = env[i];
	return (path + 5);
}

int			mini_env(char **args, char **env)
{
	(void)args;
	(void)env;
	return (1);
}
