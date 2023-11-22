/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 19:02:16 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 19:02:18 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_env
{
	pthread_t			*philo;
	pthread_mutex_t		*check_fork;

	pthread_mutex_t		init_id;
	pthread_mutex_t		dead_notice;

	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	unsigned long		base_time;
	int					philo_size;
	int					max_eat;

	int					*fork;
	int					*cycle_end;

	int					philo_id_iter;
	int					dead_found;
	int					dead_print;
}		t_env;

unsigned long	ft_atoul_no_protect(char *s);
unsigned long	get_time(void);
unsigned long	get_dif_time(unsigned long time);

void			ft_usleep_till(unsigned long time, t_env *env);
void			print_eating(t_env *env, int id, int *ate, unsigned long time);
void			print_dead(t_env *env, int id, unsigned long end_time);
void			print_think(t_env *env, int id, unsigned long end_time);
void			philo_take_fork(t_env *env, int id, unsigned long *end_time \
				, int *ate);
void			philo_cycle(t_env *env, int *ate, int id, \
				unsigned long end_time);
void			*fn_thread(void *arg);
void			*fn_single_thread(void *arg);
void			alone_philo(t_env *env);
void			clean_out(t_env *env);
int				ft_atoi_no_protect(char *s);
int				get_input(char *arr[], int ac, t_env *env);
int				ft_malloc_error_exit(t_env *env);
int				init_env(t_env *env);
int				ft_error_exit(t_env *env, char *s);
