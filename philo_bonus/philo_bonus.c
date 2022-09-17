/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 00:46:16 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/17 23:16:57 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_create(t_philo *philos, t_all *init)
{
	int	i;

	i = 0;
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
			if (pthread_create(&philos[i].philo, NULL \
				, routine, &philos[i]) != 0)
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
	if (init->nb_p <= 0 || init->tt_d <= 0 || init->tt_e <= 0 \
		|| init->tt_s <= 0 || (argv[5] && init->e_t <= 0))
	{
		write(2, "I only accepte positive numbers greater than 0", 47);
		exit(EXIT_FAILURE);
	}
	sem_unlink("/sem_print");
	init->printing = sem_open("/sem_print", O_CREAT | O_RDWR, 0600, 1);
	sem_unlink("/forks");
	init->forks = sem_open("/forks", O_CREAT | O_RDWR, 0600, init->nb_p);
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
		ft_create(philos, init);
		ft_wait(philos);
	}
	else
		write (2, "Error in passed args", 21);
	return (0);
}
