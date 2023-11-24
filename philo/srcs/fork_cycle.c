/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:42:04 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:42:06 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

static void	remove_fork(t_env *env, int id)
{
	env->fork[id - 1] = 0;
	env->fork[id % env->philo_size] = 0;
	env->cycle_end[id - 1] -= 1;
}

static void	add_fork_back(t_env *env, int id)
{
	env->fork[id - 1] = 1;
	env->fork[id % env->philo_size] = 1;
}

void	philo_take_fork(t_env *env, int id, unsigned long *end_time, int *ate)
{
	int	dead;

	dead = 0;
	remove_fork(env, id);
	if (env->time_to_eat > get_dif_time(*end_time))
		dead = 1;
	if (env->stop_work == 0)
		print_eating(env, id, ate, *end_time);
	if (dead)
		print_dead(env, id, *end_time);
	else
	{
		ft_usleep_till(get_time() + env->time_to_eat, env);
		*end_time = get_time() + env->time_to_die;
		add_fork_back(env, id);
	}
	pthread_mutex_unlock(&env->check_fork[id - 1]);
	pthread_mutex_unlock(&env->check_fork[id % env->philo_size]);
}

static void	run_cycle(t_env *env, unsigned long end_time, int *ate, int id)
{
	if (env->time_to_sleep + get_time() > end_time)
	{
		pthread_mutex_unlock(&env->dead_notice);
		ft_usleep_till(end_time, env);
		pthread_mutex_lock(&env->dead_notice);
		if (env->stop_work == 0)
		{
			env->stop_work = 1;
			printf("\033[0;31m%lu %d has dead\033[0m\n", \
				get_dif_time(env->base_time), id);
		}
		pthread_mutex_unlock(&env->dead_notice);
	}
	else
	{
		pthread_mutex_unlock(&env->dead_notice);
		*ate = 0;
		ft_usleep_till(env->time_to_sleep + get_time(), env);
	}
	pthread_mutex_lock(&env->dead_notice);
	if (env->stop_work == 0 && get_dif_time(end_time) != 0)
		printf("\033[0;37m%lu %d is thinking\033[0m\n", \
			get_dif_time(env->base_time), id);
	pthread_mutex_unlock(&env->dead_notice);
	usleep(100);
}

void	philo_cycle(t_env *env, int *ate, int id, unsigned long end_time)
{
	if (*ate == 1 && env->stop_work == 0 && env->cycle_end[id - 1]
		&& end_time > get_time())
	{
		printf("\033[0;34m%lu %d is sleeping\033[0m\n", \
			get_dif_time(env->base_time), id);
		run_cycle(env, end_time, ate, id);
	}
	else
		pthread_mutex_unlock(&env->dead_notice);
}
