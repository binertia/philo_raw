#include "../includes/philosopher.h"

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

unsigned long	get_dif_time(unsigned long time)
{
	long temp;

	temp = (long)get_time() - time;
	if (temp < 0)
		return (temp * -1);
	return (temp);
}

void ft_usleep_till(unsigned long time, t_env *env)
{
	while (get_time() < time && env->dead_print == 0)
		usleep(200);
}
