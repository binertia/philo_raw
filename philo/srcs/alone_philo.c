/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alone_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:31:35 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:31:37 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	*fn_single_thread(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	ft_usleep_till(env->time_to_die + get_time(), env);
	usleep(1);
	printf("\033[0;31m%lu 1 is dead\n\033[0m", get_dif_time(env->base_time));
	return (arg);
}

void	alone_philo(t_env *env)
{
	pthread_create(&env->philo[0], NULL, &fn_single_thread, env);
	pthread_join(env->philo[0], NULL);
}
