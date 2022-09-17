/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 17:54:31 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/17 21:41:54 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_creat(t_philo *philos, t_all *init)
{
	int	i;

	i = 0;
	while (i < philos->all->nb_p)
	{
		if (pthread_create(&philos[i].philo, NULL, routine, &philos[i]) != 0)
			return (EXIT_FAILURE);
		if (pthread_detach(philos[i].philo) != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	usleep(25 * init->nb_p);
	i = 1;
	while (i < philos->all->nb_p)
	{
		if (pthread_create(&philos[i].philo, NULL, routine, &philos[i]) != 0)
			return (EXIT_FAILURE);
		if (pthread_detach(philos[i].philo) != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	return (EXIT_SUCCESS);
}

int	ft_check_meals(t_philo *philos, t_index i)
{
	pthread_mutex_lock(&philos[i].all->check_eat);
	if (philos->all->check == philos->all->nb_p)
	{
		pthread_mutex_lock(&philos->all->r_flag);
		philos->all->flag = 0;
		pthread_mutex_unlock(&philos->all->r_flag);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&philos[i].all->check_eat);
	return (EXIT_SUCCESS);
}

int	ft_ch_flag(t_philo *philos, t_index i)
{
	if (ft_get_time() - philos[i].l_e >= philos[i].all->tt_d)
	{
		pthread_mutex_lock(&philos->all->r_flag);
		philos->all->flag = 0;
		pthread_mutex_unlock(&philos->all->r_flag);
		printf("%ld %d died\n", ft_get_time() - philos->all->simul, \
			philos->index);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	ft_supervisor(t_philo *philos)
{
	t_index	i;

	while (1)
	{
		i = 0;
		while (i < philos->all->nb_p)
		{
			pthread_mutex_lock(&philos->all->printing);
			if (philos->all->e_t != -1 && ft_check_meals(philos, i) == EXIT_FAILURE)
				return ;
			pthread_mutex_lock(&philos[i].read_meals);
			if (ft_ch_flag(philos, i) == EXIT_FAILURE)
				return ;
			pthread_mutex_unlock(&philos->all->printing);
			pthread_mutex_unlock(&philos[i].read_meals);
			i++;
		}
		usleep(50);
	}
}
