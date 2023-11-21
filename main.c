#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

// struct  -> child struct 
// struct that send to pthread_create should contain : mutex, base ,numbers of philo (this is variious point)
// send iterate num and send array of int arr[] and iterate it in mutex
//
// case of 5; how 5 can eat first in 3rd round;
// // track how many time that philo ate;
// // // logic : if (n_round % 2 != 0) while (t_data-> ate_n_time < n_round / 2) then start eat;
// case 2 : 2 need to get (2 fork and 2 + 1 % n_philo); philo_mutex_lock fork and unlock when finish eat


// args : number_of_philosophers , time_to_die , time_to_eat , time_to_sleep , [number_of_times_each_philosopher_must_eat]

// all time is millisec;

// ft_atoi =======

int	ft_atoi_no_protect(char *s)
{
	long res;

	res = 0;
	while (*s && *s >= '0' && *s <= '9')
		res = (res * 10) + (*s++ - '0');
	return ((int)res);
}

unsigned long	ft_atoul_no_protect(char *s)
{
	unsigned long res;

	res = 0;
	while (*s && *s >= '0' && *s <= '9')
		res = (res * 10) + (*s++ - '0');
	return (res);
}

// ===============

typedef struct	s_env
{
	int	philo_size;
	int	max_eat;
	unsigned long time_to_die;
	unsigned long time_to_eat;
	unsigned long time_to_sleep;
	unsigned long base_time;

	pthread_t	*philo;
	pthread_mutex_t init_id;
	pthread_mutex_t check_fork;
	pthread_mutex_t dead_notice;
	int	*fork;
	int	philo_id_iter;

	int dead_found;
	int	dead_print;
	int *cycle_end;
}		t_env;

int	check_input(char *s)
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

int	valid_input(t_env *env)
{
	if (env->philo_size == 0 || env->time_to_die == 0
		||env->time_to_eat == 0 || env->time_to_sleep == 0)
		return (2);
	if (env->max_eat == 0)
		return (2);
	return (0);
}

int ft_input_error_exit(char *s)
{
	printf("%s", s);
	return (2);
}

int	get_input(char *arr[], int ac, t_env *env)
{
	if (check_input(arr[1]) || check_input(arr[2])
		|| check_input(arr[3]) || check_input(arr[4]))
		return (ft_input_error_exit("Error : bad input\n"));
	if (ac == 6)
	{
		if (check_input(arr[5]))
			return (ft_input_error_exit("Error : bad input\n"));
		env->max_eat = ft_atoi_no_protect(arr[5]);
	}
	else
		env->max_eat = -1; // no optaional for arr[5]
	env->philo_size = ft_atoi_no_protect(arr[1]);
	env->time_to_die = ft_atoul_no_protect(arr[2]);
	env->time_to_eat = ft_atoul_no_protect(arr[3]);
	env->time_to_sleep = ft_atoul_no_protect(arr[4]);
	if (valid_input(env))
		return (ft_input_error_exit("Error : input value error\n"));
	return (0);
}

// ===== get_time ====

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

//  =========  start thread ======

void	set_philo(t_env *env, int *id, unsigned long *end_time, int *ate)
{
	*ate = 0;
	pthread_mutex_lock(&env->init_id);
	*id = env->philo_id_iter;
	env->philo_id_iter++;
	*end_time = env->base_time + env->time_to_die;
	pthread_mutex_unlock(&env->init_id);
}

void ft_usleep_till(unsigned long time)
{
	while (get_time() < time)
	{
		usleep(100);
	}
}

void	update_dead(t_env *env, int *dead)
{
		env->dead_found = 1;
		*dead = 1;
}

void	print_eating(t_env *env, int id, int *ate)
{
	*ate = 1;
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", get_dif_time(env->base_time), id);
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", get_dif_time(env->base_time), id);
	printf("\033[0;32m%lu %d is eating\n\033[0m", get_dif_time(env->base_time), id);
}

void	remove_fork(t_env *env, int id)
{
	env->fork[id - 1] = 0;
	env->fork[id % env->philo_size] = 0;
	env->cycle_end[id - 1] -= 1;
}

void	add_fork_back(t_env *env, int id)
{
	pthread_mutex_lock(&env->check_fork);
	env->fork[id - 1] = 1;
	env->fork[id % env->philo_size] = 1;
	pthread_mutex_unlock(&env->check_fork);
}

void	print_dead(t_env *env, int id, unsigned long end_time)
{
	pthread_mutex_lock(&env->dead_notice);
	if (env->dead_print == 0)
	{
		ft_usleep_till(end_time);
		printf("\033[0;31m%lu %d has dead\n\033[0m", get_dif_time(env->base_time), id);
		env->dead_print = 1;
	}
	pthread_mutex_unlock(&env->dead_notice);
}

void	philo_take_fork(t_env *env, int id, unsigned long *end_time, int *ate)
{
	int	dead;

	dead = 0;
	remove_fork(env, id);
	if (env->time_to_eat > get_dif_time(*end_time))
		update_dead(env, &dead);
	pthread_mutex_unlock(&env->check_fork);
	print_eating(env, id, ate);
	if (dead)
		print_dead(env, id, *end_time);
	else
	{
		ft_usleep_till(get_time() + env->time_to_eat);
		*end_time = get_time() + env->time_to_die;	
	}
	add_fork_back(env, id);
}

void philo_cycle(t_env *env,int *ate, int id, unsigned long end_time)
{
	if (*ate == 1 && env->dead_print == 0 && env->cycle_end[id - 1])
	{
		printf("\033[0;34m%lu %d is sleeping\n\033[0m", get_dif_time(env->base_time),  id);
		if (env->time_to_sleep > get_dif_time(end_time))
		{
			env->dead_found = 1;
			pthread_mutex_unlock(&env->dead_notice);
			ft_usleep_till(end_time);
			printf("\033[0;31m%lu %d has dead\n\033[0m",get_dif_time(env->base_time), id);
		}
		else
		{
			pthread_mutex_unlock(&env->dead_notice);
			*ate = 0;
			ft_usleep_till(env->time_to_sleep + get_time());
		}
		pthread_mutex_lock(&env->dead_notice);
		if (env->dead_found == 0)
			printf("\033[0;37m%lu %d is thinking\n\033[0m", get_dif_time(env->base_time),  id);
		pthread_mutex_unlock(&env->dead_notice);
		usleep(200);
	}
	else
		pthread_mutex_unlock(&env->dead_notice);
}

// === case 1 n philo
//

// ---00000000000000000000
void	*fn_thread(void *arg)
{
	int	philo_id;
	int ate;
	int	even_que;
	t_env	*env;
	unsigned long end_time;

	env = (t_env *)arg;
	set_philo(env, &philo_id, &end_time, &ate);
	even_que = (philo_id % 2);
	if (!even_que)
		usleep(50);
	while (env->dead_print == 0 && env->cycle_end[philo_id - 1]) 
	{
		pthread_mutex_lock(&env->check_fork);
		if (env->fork[philo_id - 1] == 1 && env->fork[philo_id % env->philo_size] == 1)
			philo_take_fork(env, philo_id, &end_time, &ate);
		else
			pthread_mutex_unlock(&env->check_fork);
		pthread_mutex_lock(&env->dead_notice);
		philo_cycle(env, &ate, philo_id, end_time);
		if (env->dead_found)
			break;
			// pthread_detach(env->philo[philo_id - 1]);
	}
	return (arg);
}

void	clean_out(t_env *env)
{
	free(env->philo);
	free(env->fork);
	free(env->cycle_end);
	pthread_mutex_destroy(&env->init_id);
	pthread_mutex_destroy(&env->check_fork);
	pthread_mutex_destroy(&env->dead_notice);
}

int ft_malloc_error_exit(t_env *env)
{
	if (env->philo)
		free(env->philo);
	if (env->fork)
		free(env->fork);
	if (env->cycle_end)
		free(env->cycle_end);
	env->philo = NULL;
	env->fork = NULL;
	env->cycle_end = NULL;
	printf("Error : malloc error");
	return (2);
}

int init_env(t_env *env)
{
	int	i;

	env->philo = (pthread_t *)malloc(sizeof(pthread_t) * env->philo_size);
	env->fork = (int *)malloc(sizeof(int) * env->philo_size);
	env->cycle_end = (int *)malloc(sizeof(int) * env->philo_size);
	if (!env->philo || !env->fork || !env->cycle_end)
		return (ft_malloc_error_exit(env));
	env->dead_found = 0;
	env->philo_id_iter = 1;
	env->dead_print = 0;
	pthread_mutex_init(&env->init_id, NULL);
	pthread_mutex_init(&env->check_fork, NULL);
	pthread_mutex_init(&env->dead_notice, NULL);
    env->base_time = get_time(); 
	i = 0;
	while (i < env->philo_size)
	{
		env->fork[i] = 1;
		env->cycle_end[i] = env->max_eat;
		i++;
	}
	return (0);
}

void	*fn_single_thread(void *arg)
{
	t_env *env;

	env = (t_env *)arg;
	ft_usleep_till(env->time_to_die + get_time());
	printf("\033[0;31m%lu 1 is dead\n\033[0m", get_dif_time(env->base_time));
	return (arg);
}

void	alone_philo(t_env *env)
{
	pthread_create(&env->philo[0], NULL, &fn_single_thread, env);
	pthread_join(env->philo[0], NULL);
}

int	ft_error_exit(t_env *env, char *s)
{
	clean_out(env);
	printf("%s\n", s);
	return (2);
}

int	start_thread(t_env *env)
{
	int	i;
	int error;

	error = 0;
	if (init_env(env))
		return (2);
	if (env->philo_size > 1)
	{
		i = 0;
		while (i < env->philo_size)
		{
			if (pthread_create(&env->philo[i], NULL, &fn_thread, env) != 0)
				return (ft_error_exit(env, "thread create error"));
			i++;
		}
		i = 0;
		while (i < env->philo_size && error == 0)
		{
			if (pthread_join(env->philo[i], NULL) != 0)
				return (ft_error_exit(env, "thread join error"));
			i++;
		}
	}
	else
		alone_philo(env);
	clean_out(env);
	return (0);
}


// ==============
int	main(int ac, char *av[])
{
	t_env	env;

	if (ac < 5 || ac > 6)
		return (2); // arg_error
	if (get_input(av, ac, &env) != 0)
		return (2); // input_error
	if (start_thread(&env) != 0)
		return (2); // error_run;
	// :TODO: handle input overbound
	return (0);
}
