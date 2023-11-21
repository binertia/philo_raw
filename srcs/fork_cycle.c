#include "../includes/philosopher.h"

static void	update_dead(t_env *env, int *dead)
{
	env->dead_found = 1;
	*dead = 1;
}

static void	remove_fork(t_env *env, int id)
{
	env->fork[id - 1] = 0;
	env->fork[id % env->philo_size] = 0;
	env->cycle_end[id - 1] -= 1;
}

static void	add_fork_back(t_env *env, int id)
{
	pthread_mutex_lock(&env->check_fork[id - 1]);
	env->fork[id - 1] = 1;
	pthread_mutex_unlock(&env->check_fork[id - 1]);
	pthread_mutex_lock(&env->check_fork[id % env->philo_size]);
	env->fork[id % env->philo_size] = 1;
	pthread_mutex_unlock(&env->check_fork[id % env->philo_size]);
}

void	philo_take_fork(t_env *env, int id, unsigned long *end_time, int *ate)
{
	int	dead;

	dead = 0;
	pthread_mutex_lock(&env->check_fork[id % env->philo_size]);
	remove_fork(env, id);
	if (env->time_to_eat > get_dif_time(*end_time))
		update_dead(env, &dead);
	pthread_mutex_unlock(&env->check_fork[id % env->philo_size]);
	pthread_mutex_unlock(&env->check_fork[id - 1]);
		print_eating(env, id, ate, *end_time);
	if (dead)
		print_dead(env, id, *end_time);
	else
	{
		ft_usleep_till(get_time() + env->time_to_eat, env);
		*end_time = get_time() + env->time_to_die;	
		add_fork_back(env, id);
	}
}


void	philo_cycle(t_env *env,int *ate, int id, unsigned long end_time)
{
	if (*ate == 1 && env->dead_print == 0 && env->cycle_end[id - 1] && env->dead_found == 0)
	{
		printf("\033[0;34m%lu %d is sleeping\033[0m\n", get_dif_time(env->base_time),  id);
		if (env->time_to_sleep > get_dif_time(end_time))
		{
			env->dead_found = 1;
			pthread_mutex_unlock(&env->dead_notice);
			ft_usleep_till(end_time, env);
			printf("\033[0;31m%lu %d has dead\033[0m\n",get_dif_time(env->base_time), id);
		}
		else
		{
			pthread_mutex_unlock(&env->dead_notice);
			*ate = 0;
			ft_usleep_till(env->time_to_sleep + get_time(), env);
		}
		pthread_mutex_lock(&env->dead_notice);
		if (env->dead_found == 0 && env->dead_print == 0 && get_dif_time(end_time) != 0)
			printf("\033[0;37m%lu %d is thinking\033[0m\n", get_dif_time(env->base_time),  id);
		pthread_mutex_unlock(&env->dead_notice);
		usleep(200);
	}
	else
		pthread_mutex_unlock(&env->dead_notice);
}
