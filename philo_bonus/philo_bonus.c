/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 00:46:16 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/15 02:24:35 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	ft_check_meals(t_philo *philo, t_index i)
{
	if (philo->n_e >= philo->all->e_t)
		exit(PHILO_IS_FULL);
}

void	ft_check_death(t_philo *philos)
{
	if (ft_get_time(philos->l_e) >= philos->all->tt_d)
	{
		printf("%ld %d died\n", ft_get_time(philos->all->simul), philos->index);
		exit(EXIT_FAILURE);
	}
}

void	ft_supervisor(t_philo *philos, t_index i)
{
	while (1)
	{
		sem_wait(&philos->all->printing);
		ft_check_meals(philos, i);
		sem_post(&philos->all->printing);
		sem_wait(&philos[i].read_meals);
		ft_check_death(philos);
		sem_post(&philos[i].read_meals);
		usleep(200);
	}
}

void	ft_print_philo(t_philo *philos, char *str)
{
	sem_wait(&philos->all->printing);
	printf("%d %d %s\n", ft_get_time(philos->all->simul), philos->index, str);
	sem_post(&philos->all->printing);
}

void	ft_sleep(long time)
{
	long	time_to_reach;

	time_to_reach = ft_get_time() + time;
	usleep((time * 0.90) / 1000);
	while (ft_get_time() < time_to_reach)
		usleep(50);
}

void	*routine(void *p)
{
	t_philo *philos;

	philos = (t_philo *)p;
	if (philos->index % 2 != 0)
		usleep(500);
	while (1)
	{
		sem_wait(&philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		sem_wait(&philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		ft_print_philo(philos, "is eating");
		gettimeofday(&philos->l_e, NULL);
		ft_sleep(&philos->all->tt_e);
		sem_wait(&philos->read_meals);
		philos->n_e++;
		sem_post(&philos->read_meals);
		sem_post(&philos->all->forks);
		sem_post(&philos->all->forks);
		ft_print_philo(philos, "is sleeping");
		ft_sleep(&philos->all->tt_s);
		ft_print_philo(philos, "is thinking");
	}
	return (0);
}

int	ft_create(t_philo *philos, t_all *init)
{
	int i = 0;
	
	init->pid = malloc(sizeof(pid_t) * init->nb_p);
	while (i < init->nb_p)
	{
		init->pid[i] = fork();
		if (init->pid[i] == -1)
		{
			write(2, "Process failed", 15);
			exit(EXIT_FAILURE);
		}
		if (init->pid[i] == 0)
		{
			if (pthread_create(&philos[i].philo, NULL, routine, &philos[i]) != 0)
				exit(EXIT_FAILURE);
			if (pthread_detach(philos[i].philo) != 0)
				exit(EXIT_FAILURE);
			ft_supervisor(philos, i);
			exit (99);
		}
		i++;
	}
	// printf("%d\n", i);
	return (EXIT_SUCCESS);
}

long	ft_get_time(t_time  last)
{
	long			final;
	long			helper;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	final = (curr.tv_sec * 1000) + (curr.tv_usec / 1000);
	helper = (last.tv_sec * 1000) + (last.tv_usec / 1000);
	return (final - helper);
}

void	ft_init_philo(t_philo *philos, t_all *init)
{
	int	i;
	char *name;

	name = ft_strdup("/read...");
	i = 0;
	gettimeofday(&init->simul, NULL);
	while (i < init->nb_p)
	{	
		philos[i].index = i + 1;
		philos[i].flag = 1;
		philos[i].l_e = init->simul;
		philos[i].n_e = 0;
		name[5] = i / 100 + '0';
		name[6] = (i / 10) % 10 + '0';
		name[7] = i % 10 + '0';
		sem_unlink(name);
		philos[i].read_meals = sem_open("/read", O_CREAT | O_RDWR, 0600, 1);
		philos[i].all = init;
		i++;
	}
	exit (150);
}

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

int	ft_init_data(char **argv, t_all *init)
{
	init->nb_p = ft_atoi(argv[1]);
	init->tt_d = ft_atoi(argv[2]);
	init->tt_e = ft_atoi(argv[3]);
	init->tt_s = ft_atoi(argv[4]);
	// init->flag = 1;
	// init->check = 0;
	if (argv[5])
		init->e_t = ft_atoi(argv[5]);
	else
		init->e_t = -1;
	if (init->nb_p <= 0 || init->tt_d <= 0 || init->tt_e <= 0 || init->tt_s <= 0 || (argv[5] && init->e_t <=0))
	{
		write(2, "I only accepte positive numbers greater than 0", 47);
		exit(EXIT_FAILURE);
	}
	sem_unlink("/sem_print");
	init->printing =  sem_open("/sem_print", O_CREAT | O_RDWR, 0600, 1);
	sem_unlink("/forks");
	init->forks =  sem_open("/forks", O_CREAT | O_RDWR, 0600, init->nb_p);
	return (EXIT_SUCCESS); 
}

int	main(int argc, char **argv)
{
	t_all	*init;
	t_philo	*philos;
	if (argc == 5 || argc == 6)
	{
		init = malloc(sizeof(t_all));
		ft_init_data(argv, init);
		philos = malloc(sizeof(t_philo) * init->nb_p);
		ft_init_philo(philos, init);
		ft_creat(philos ,init);
		// ft_supervisor(philos);
		// return (EXIT_FAILURE);
	}
	else
		write (2, "Error in passed args", 21);
	return (EXIT_FAILURE);
}