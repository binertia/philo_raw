/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:42:34 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:42:36 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

static void	ft_mutex_destroy(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->philo_size)
	{
		pthread_mutex_destroy(&env->check_fork[i]);
		i++;
	}
	if (env->check_fork)
		free(env->check_fork);
	env->check_fork = NULL;
	pthread_mutex_destroy(&env->init_id);
	pthread_mutex_destroy(&env->dead_notice);
}

static void	ft_free_malloc(t_env *env)
{
	if (env->philo)
		free(env->philo);
	if (env->fork)
		free(env->fork);
	if (env->cycle_end)
		free(env->cycle_end);
	if (env->check_fork)
		free(env->check_fork);
	env->philo = NULL;
	env->fork = NULL;
	env->cycle_end = NULL;
	env->check_fork = NULL;
}

void	clean_out(t_env *env)
{
	ft_mutex_destroy(env);
	ft_free_malloc(env);
}

int	ft_malloc_error_exit(t_env *env)
{
	ft_free_malloc(env);
	printf("Error : malloc error");
	return (2);
}

int	ft_error_exit(t_env *env, char *s)
{
	clean_out(env);
	printf("%s\n", s);
	return (2);
}
