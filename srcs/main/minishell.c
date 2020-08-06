/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macrespo <macrespo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/21 11:51:22 by cclaude           #+#    #+#             */
/*   Updated: 2020/08/06 17:01:20 by macrespo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	redir_and_exec(t_mini *mini, t_token *token)
{
	t_token	*prev;
	t_token	*next;
	int		pipe;

	prev = prev_sep(token, NOSKIP);
	next = next_sep(token, NOSKIP);
	pipe = 0;
	if (is_type(prev, TRUNC))
		redir(mini, token, TRUNC);
	else if (is_type(prev, APPEND))
		redir(mini, token, APPEND);
	else if (is_type(prev, INPUT))
		input(mini, token);
	else if (is_type(prev, PIPE))
		pipe = minipipe(mini);
	if (next && is_type(next, END) == 0 && pipe != 1)
		redir_and_exec(mini, next->next);
	if ((is_type(prev, END) || is_type(prev, PIPE) || !prev) && pipe != 1)
		exec_cmd(mini, token);
	if (pipe == 2)
		exit(0);
	if (has_pipe(token))
		waitpid(mini->pid, NULL, 0);
}

void	minishell(t_mini *mini)
{
	t_token	*token;
	pid_t	pid;
	int		status;

	token = next_run(mini->start, NOSKIP);
	while (mini->exit == 0 && is_type(token, CMD))
	{
		pid = (has_pipe(token)) ? fork() : 1;
		if (pid == 0 || pid == 1)
			redir_and_exec(mini, token);
		else
			waitpid(pid, &status, 0);
		if (pid == 0)
			exit(mini->exit);
		reset_std(mini);
		close_fds(mini);
		waitpid(mini->pid, NULL, 0);
		reset_fds(mini);
		token = next_run(token, SKIP);
	}
}

void	init_sig(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}

static void increment_shell_level(t_env *env)
{
	int		shell_level;
	char	env_name[BUFF_SIZE];
	char	*SHLVL;

	shell_level = ft_atoi(get_env_value("SHLVL", env)) + 1;
	while (env && env->next)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp("SHLVL", env_name) == 0)
		{
			ft_memdel(env->value);
			SHLVL = ft_itoa(shell_level);
			env->value = ft_strjoin("SHLVL=", SHLVL);
			ft_memdel(SHLVL);
			return ;
		}
		env = env->next;
	}
}

int		main(int ac, char **av, char **env)
{
	t_mini	mini;

	(void)ac;
	(void)av;
	mini.in = dup(STDIN);
	mini.out = dup(STDOUT);
	mini.exit = 0;
	mini.ret = 0;
	reset_fds(&mini);
	env_init(&mini, env);
	increment_shell_level(mini.env);
	while (mini.exit == 0)
	{
		init_sig();
		parse(&mini);
		if (mini.start != NULL)
			minishell(&mini);
		free_token(mini.start);
	}
	return (0);
}
