#include "../includes/philosopher.h"

void	print_eating(t_env *env, int id, int *ate, unsigned long end_time)
{
	unsigned long	time;

	*ate = 1;
	time = get_dif_time(env->base_time);
	if (get_time() >= end_time)
		return ;
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", time, id);
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", time, id);
	printf("\033[0;32m%lu %d is eating\n\033[0m", time, id);
}

void	print_dead(t_env *env, int id, unsigned long end_time)
{
	pthread_mutex_lock(&env->dead_notice);
	if (env->dead_print == 0)
	{
		ft_usleep_till(end_time, env);
		printf("\033[0;31m%lu %d has dead\n\033[0m", get_dif_time(env->base_time), id);
		env->dead_print = 1;
	}
	pthread_mutex_unlock(&env->dead_notice);
}
