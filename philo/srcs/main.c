/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:46:28 by ksongchu          #+#    #+#             */
/*   Updated: 2023/11/22 20:46:30 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

static int	start_thread(t_env *env)
{
	int	i;

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
		while (i < env->philo_size)
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

int	main(int ac, char *av[])
{
	t_env	env;

	if (ac < 5 || ac > 6)
	{
		write(STDERR_FILENO, "Error : bad input\n", 18);
		return (2);
	}
	if (get_input(av, ac, &env) != 0)
		return (2);
	if (start_thread(&env) != 0)
		return (2);
	printf("%lu :: process exit\n", get_dif_time(env.base_time));
	return (0);
}
