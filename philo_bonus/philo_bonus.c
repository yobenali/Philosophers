/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 00:46:16 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/15 23:20:35 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

char	*ft_strdup(const char *str)
{
	int		i;
	char	*tab;

	i = 0;
	while (str[i])
		i++;
	tab = malloc(sizeof(char) * (i + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (str[i])
	{
		tab[i] = str[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

void	ft_check_meals(t_philo *philo)
{
	if (philo->all->e_t != -1 && philo->n_e >= philo->all->e_t)
	{
		printf("yo this is philo nbr ; %d\n", philo->index);
		exit(PHILO_IS_FULL);
	}
}

void	ft_check_death(t_philo *philos)
{
	if (ft_get_time(philos->l_e) >= philos->all->tt_d)
	{
		printf("%ld %d died\n", ft_get_time(philos->all->simul), philos->index + 1);
		exit(EXIT_FAILURE);
	}
}

void	ft_supervisor(t_philo *philos, t_index i)
{
	while (1)
	{
		sem_wait(philos->all->printing);
		// sem_wait(philos[i].read_meals);
		// ft_check_meals(philos + i);
		// sem_post(philos[i].read_meals);
		ft_check_death(philos + i);
		sem_post(philos->all->printing);
		usleep(200);
	}
}

void	ft_print_philo(t_philo *philos, char *str)
{
	sem_wait(philos->all->printing);
	printf("%ld %d %s\n", ft_get_time(philos->all->simul), philos->index + 1, str);
	sem_post(philos->all->printing);
}

void	ft_sleep(long time)
{
	long			time_to_reach;
	long			final;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	final = (curr.tv_sec * 1000) + (curr.tv_usec / 1000);
	time_to_reach = final + time;
	usleep((time * 0.90) * 1000);
	while (final < time_to_reach)
	{
		usleep(50);
		gettimeofday(&curr, NULL);
		final = (curr.tv_sec * 1000) + (curr.tv_usec / 1000);
	}
}

void	*routine(void *p)
{
	t_philo	*philos;

	philos = (t_philo *)p;
	if (philos->index % 2 != 0)
		usleep(1000);
	while (1)
	{
		sem_wait(philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		sem_wait(philos->all->forks);
		ft_print_philo(philos, "has taken a fork");
		ft_print_philo(philos, "is eating");
		gettimeofday(&philos->l_e, NULL);
		ft_sleep(philos->all->tt_e);
		sem_wait(philos->read_meals);
		philos->n_e++;
		if (philos->all->e_t != -1 && philos->n_e >= philos->all->e_t)
			exit (PHILO_IS_FULL);
		sem_post(philos->read_meals);
		sem_post(philos->all->forks);
		sem_post(philos->all->forks);
		ft_print_philo(philos, "is sleeping");
		ft_sleep(philos->all->tt_s);
		ft_print_philo(philos, "is thinking");
	}
	return (0);
}

int	ft_create(t_philo *philos, t_all *init)
{
	int	i;

	i = 0;
	init->pid = malloc(sizeof(pid_t) * init->nb_p);
	while (i < init->nb_p)
	{
		// printf("yo from inside\n");
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
	return (EXIT_SUCCESS);
}

long	ft_get_time(t_time last)
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
	int		i;
	char	*name;

	i = 0;
	name = ft_strdup("/read...");
	gettimeofday(&init->simul, NULL);
	while (i < init->nb_p)
	{	
		philos[i].index = i;
		philos[i].flag = 1;
		philos[i].l_e = init->simul;
		philos[i].n_e = 0;
		name[5] = i / 100 + '0';
		name[6] = (i / 10) % 10 + '0';
		name[7] = i % 10 + '0';
		sem_unlink(name);
		philos[i].read_meals = sem_open(name, O_CREAT | O_RDWR, 0600, 1);
		philos[i].all = init;
		i++;
	}
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
	if (argv[5])
		init->e_t = ft_atoi(argv[5]);
	else
		init->e_t = -1;
	if (init->nb_p <= 0 || init->tt_d <= 0 || init->tt_e <= 0 || init->tt_s <= 0 || (argv[5] && init->e_t <= 0))
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

void	ft_wait(t_philo *philos)
{
	pid_t	pid;
	int		status;
	t_index	i;
	t_index	j;

	j = -1;
	while (++j < philos->all->nb_p)
	{
		pid = waitpid(-1, &status, 0);
		if (pid != -1 && WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == EXIT_FAILURE)
			{
				i = -1;
				while (++i < philos->all->nb_p)
				{
					if (philos->all->pid[i] != pid)
						kill(philos->all->pid[i], SIGKILL);
				}
				exit (1);
			}
		}
	}
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
		ft_create(philos, init);
		ft_wait(philos);
	}
	else
		write (2, "Error in passed args", 21);
	return (0);
}
