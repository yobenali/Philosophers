/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 19:29:09 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/10 17:44:10 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	char	digit;
	int		res;
	int		max;
	int		i;

	res = 0;
	i = 0;
	max = INT_MAX / 10;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (res > max || (res == max && digit > 7))
			return (0);
		res = res * 10 + digit;
		i++;
	}
	return (res);
}

long	ft_get_time(void)
{
	long			final;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	final = (curr.tv_sec * 1000) + (curr.tv_usec / 1000);
	return (final);
}

void	ft_init_philo(t_philo *philos, t_all *init)
{
	int	i;

	i = 0;
	init->simul = ft_get_time();
	while (i < init->nb_p)
	{	
		philos[i].index = i + 1;
		philos[i].l_e = init->simul;
		philos[i].n_e = 0;
		pthread_mutex_init(&philos[i].fork, NULL);
		pthread_mutex_init(&philos[i].read_meals, NULL);
		philos[i].nxt_fork = &philos[(i + 1) % init->nb_p].fork;
		philos[i].all = init;
		i++;
	}
}

int	ft_check_flag(t_philo *philos)
{
	pthread_mutex_lock(&philos->all->r_flag);
	if (philos->all->flag == 0)
	{
		pthread_mutex_unlock(&philos->all->r_flag);
		return (0);
	}
	pthread_mutex_unlock(&philos->all->r_flag);
	return (1);
}

void	ft_print_philo(t_philo *philos, char *str)
{
	pthread_mutex_lock(&philos->all->printing);
	printf("%ld %d %s\n", ft_get_time() - philos->all->simul \
		, philos->index, str);
	pthread_mutex_unlock(&philos->all->printing);
}

void	ft_sleep(long time)
{
	long	time_to_reach;

	time_to_reach = ft_get_time() + time;
	usleep(time * 0.90 / 1000);
	while (ft_get_time() < time_to_reach)
		usleep(37);
}

void	*routine(void *p)
{
	t_philo	*philos;

	philos = (t_philo *)p;
	while (ft_check_flag(philos))
	{
		pthread_mutex_lock(&philos->fork);
		ft_print_philo(philos, "has taken a fork");
		pthread_mutex_lock(philos->nxt_fork);
		ft_print_philo(philos, "has taken a fork");
		ft_print_philo(philos, "is eating");
		pthread_mutex_lock(&philos->read_meals);
		philos->l_e = ft_get_time();
		pthread_mutex_unlock(&philos->read_meals);
		ft_sleep(philos->all->tt_e);
		pthread_mutex_lock(&philos->read_meals);
		philos->n_e++;
		if (philos->n_e >= philos->all->e_t)
		{
			pthread_mutex_lock(&philos->all->check_eat);
			philos->all->check++;
			pthread_mutex_unlock(&philos->all->check_eat);
		}
		pthread_mutex_unlock(&philos->read_meals);
		pthread_mutex_unlock(philos->nxt_fork);
		pthread_mutex_unlock(&philos->fork);
		ft_print_philo(philos, "is sleeping");
		ft_sleep(philos->all->tt_s);
		ft_print_philo(philos, "is thinking");
	}
	return (0);
}

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

int	ft_init_data(char **argv, t_all *init)
{
	init->nb_p = ft_atoi(argv[1]);
	init->tt_d = ft_atoi(argv[2]);
	init->tt_e = ft_atoi(argv[3]);
	init->tt_s = ft_atoi(argv[4]);
	init->flag = 1;
	init->check = 0;
	if (argv[5])
		init->e_t = ft_atoi(argv[5]);
	else
		init->e_t = -1;
	if (init->nb_p <= 0 || init->tt_d <= 0 || init->tt_e <= 0 \
		|| init->tt_s <= 0 || (argv[5] && init->e_t <= 0))
	{
		write (2, "I only accepte positive numbers greater than 1", 47);
		return (EXIT_FAILURE);
	}
	pthread_mutex_init(&init->printing, NULL);
	pthread_mutex_init(&init->r_flag, NULL);
	pthread_mutex_init(&init->check_eat, NULL);
	return (EXIT_SUCCESS);
}

int	ft_check_meals(t_philo *philos, index i)
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

void	ft_supervisor(t_philo *philos)
{
	index	i;

	while (1)
	{
		i = 0;
		while (i < philos->all->nb_p)
		{
			pthread_mutex_lock(&philos->all->printing);
			if (ft_check_meals(philos, i) == EXIT_FAILURE)
				return ;
			pthread_mutex_lock(&philos[i].read_meals);
			if (ft_get_time() - philos[i].l_e >= philos[i].all->tt_d)
			{
				pthread_mutex_lock(&philos->all->r_flag);
				philos->all->flag = 0;
				pthread_mutex_unlock(&philos->all->r_flag);
				printf("%ld %d died\n", ft_get_time() - philos->all->simul, \
					philos->index);
				return ;
			}
			pthread_mutex_unlock(&philos->all->printing);
			pthread_mutex_unlock(&philos[i].read_meals);
			i++;
		}
		usleep(50);
	}
}

int	main(int argc, char **argv)
{
	t_all	*init;
	t_philo	*philos;

	if (argc == 5 || argc == 6)
	{
		init = malloc(sizeof(t_all));
		if (ft_init_data(argv, init) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		philos = malloc(sizeof(t_philo) * init->nb_p);
		ft_init_philo(philos, init);
		if (ft_creat(philos, init) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		ft_supervisor(philos);
		return (EXIT_FAILURE);
	}
	else
		write (2, "Error in passed args", 21);
	return (EXIT_FAILURE);
}
