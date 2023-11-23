/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:49:19 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:52:24 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

static void	set_philo(t_env *env, int *id, unsigned long *end_time, int *ate)
{
	*ate = 0;
	pthread_mutex_lock(&env->init_id);
	*id = env->philo_id_iter;
	env->philo_id_iter++;
	*end_time = env->base_time + env->time_to_die;
	pthread_mutex_unlock(&env->init_id);
}

static void	thread_logic(t_env *env, int id, unsigned long *end_time, int *ate)
{
	while (env->dead_print == 0 && env->cycle_end[id - 1])
	{
		pthread_mutex_lock(&env->check_fork[id - 1]);
		if (env->fork[id - 1] == 1)
		{
			pthread_mutex_lock(&env->check_fork[id % env->philo_size]);
			if (env->fork[id % env->philo_size] == 1)
				philo_take_fork(env, id, end_time, ate);
			else
			{
				pthread_mutex_unlock(&env->check_fork[id % env->philo_size]);
				pthread_mutex_unlock(&env->check_fork[id - 1]);
			}
		}
		else
			pthread_mutex_unlock(&env->check_fork[id - 1]);
		pthread_mutex_lock(&env->dead_notice);
		philo_cycle(env, ate, id, *end_time);
		if (env->dead_print)
			break ;
	}
}

void	*fn_thread(void *arg)
{
	int				philo_id;
	int				ate;
	int				even_que;
	t_env			*env;
	unsigned long	end_time;

	env = (t_env *)arg;
	set_philo(env, &philo_id, &end_time, &ate);
	even_que = (philo_id % 2);
	if (!even_que)
	{
		if (env->philo_size > 80 && env->philo_size < 150)
			usleep(2000);
		else if (env->philo_size >= 150)
			usleep(3000);
		usleep(400);
	}
	thread_logic(env, philo_id, &end_time, &ate);
	return (arg);
}

int	init_env(t_env *env)
{
	int	i;

	env->philo = (pthread_t *)malloc(sizeof(pthread_t) * env->philo_size);
	env->fork = (int *)malloc(sizeof(int) * env->philo_size);
	env->cycle_end = (int *)malloc(sizeof(int) * env->philo_size);
	env->check_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) \
		* env->philo_size);
	if (!env->philo || !env->fork || !env->cycle_end)
		return (ft_malloc_error_exit(env));
	env->dead_found = 0;
	env->philo_id_iter = 1;
	env->dead_print = 0;
	pthread_mutex_init(&env->init_id, NULL);
	pthread_mutex_init(&env->dead_notice, NULL);
	i = 0;
	while (i < env->philo_size)
	{
		env->fork[i] = 1;
		env->cycle_end[i] = env->max_eat;
		pthread_mutex_init(&env->check_fork[i], NULL);
		i++;
	}
	env->base_time = get_time();
	return (0);
}
