#include "../includes/philosopher.h"

static int	check_input(char *s)
{
	if (!*s)
		return (2);
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			s++;
		else
			break;
	}
	if (*s)
		return (2);
	return (0);
}

static int	valid_input(t_env *env)
{
	if (env->philo_size == 0 || env->time_to_die == 0
		||env->time_to_eat == 0 || env->time_to_sleep == 0 || env->max_eat == 0)
		return (2);
	if ((int)env->philo_size == -2 || (long)env->max_eat == -2 || (long)env->time_to_die == -2
		|| (long)env->time_to_eat == -2 || (long)env->time_to_sleep == -2 )
		return (2);
	return (0);
}

static int input_error_exit(char *s)
{
	printf("%s", s);
	return (2);
}

int	get_input(char *arr[], int ac, t_env *env)
{
	if (check_input(arr[1]) || check_input(arr[2])
		|| check_input(arr[3]) || check_input(arr[4]))
		return (input_error_exit("Error : bad input\n"));
	if (ac == 6)
	{
		if (check_input(arr[5]))
			return (input_error_exit("Error : bad input\n"));
		env->max_eat = ft_atoi_no_protect(arr[5]);
	}
	else
		env->max_eat = -1; // no optaional for arr[5]
	env->philo_size = ft_atoi_no_protect(arr[1]);
	env->time_to_die = ft_atoul_no_protect(arr[2]);
	env->time_to_eat = ft_atoul_no_protect(arr[3]);
	env->time_to_sleep = ft_atoul_no_protect(arr[4]);
	if (valid_input(env))
		return (input_error_exit("Error : input value error -- value is out of bound\n"));
	return (0);
}
