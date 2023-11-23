/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:48:02 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:48:04 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	print_eating(t_env *env, int id, int *ate, unsigned long end_time)
{
	*ate = 1;
	if (get_time() >= end_time)
		return ;
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", \
		get_dif_time(env->base_time), id);
	printf("\033[0;33m%lu %d has taken a fork\n\033[0m", \
		get_dif_time(env->base_time), id);
	printf("\033[0;32m%lu %d is eating\n\033[0m", \
		get_dif_time(env->base_time), id);
}

void	print_dead(t_env *env, int id, unsigned long end_time)
{
	pthread_mutex_lock(&env->dead_notice);
	if (env->dead_print == 0)
	{
		ft_usleep_till(end_time, env);
		env->dead_print = 1;
		printf("\033[0;31m%lu %d has dead\n\033[0m", \
			get_dif_time(env->base_time), id);
	}
	pthread_mutex_unlock(&env->dead_notice);
}
