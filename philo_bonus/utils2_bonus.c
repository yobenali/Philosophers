/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 23:10:40 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/17 23:20:59 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	ft_print_philo(t_philo *philos, char *str)
{
	sem_wait(philos->all->printing);
	printf("%ld %d %s\n", ft_get_time(philos->all->simul) \
		, philos->index + 1, str);
	sem_post(philos->all->printing);
}

void	ft_check_meals(t_philo *philo)
{
	if (philo->all->e_t != -1 && philo->n_e >= philo->all->e_t)
		philo->flag = 0;
}

void	ft_check_death(t_philo *philos)
{
	if (ft_get_time(philos->l_e) >= philos->all->tt_d)
	{
		printf("%ld %d died\n", ft_get_time(philos->all->simul) \
			, philos->index + 1);
		exit(EXIT_FAILURE);
	}
}

void	ft_supervisor(t_philo *philos, t_index i)
{
	while (1)
	{
		sem_wait(philos->all->printing);
		sem_wait(philos[i].read_meals);
		ft_check_meals(philos + i);
		sem_post(philos[i].read_meals);
		ft_check_death(philos + i);
		sem_post(philos->all->printing);
		usleep(200);
	}
}

void	*routine(void *p)
{
	t_philo	*philos;

	philos = (t_philo *)p;
	(philos->index % 2 != 0) && usleep(1000);
	while (1)
	{
		sem_wait(philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		sem_wait(philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		ft_print_philo(philos, "is eating");
		gettimeofday(&philos->l_e, NULL);
		ft_sleep(philos->all->tt_e);
		sem_post(philos->all->forks);
		sem_post(philos->all->forks);
		sem_wait(philos->read_meals);
		philos->n_e++;
		if (philos->flag == 0)
			exit (PHILO_IS_FULL);
		sem_post(philos->read_meals);
		ft_print_philo(philos, "is sleeping");
		ft_sleep(philos->all->tt_s);
		ft_print_philo(philos, "is thinking");
	}
	return (0);
}
