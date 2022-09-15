/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 00:48:18 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/15 02:20:49 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>
# include <string.h>

typedef int	t_index;
typedef	struct timeval t_time;

#define PHILO_IS_FULL 3
typedef struct s_all
{
	int				nb_p;
	int				tt_d;
	int				tt_e;
	int				tt_s;
	int				e_t;
	int				pid;
	t_time			simul;
	sem_t       	*printing;
	sem_t	        *forks;
}	t_all;

typedef struct s_philo
{
	int				n_e;
	int				flag;
	int				index;
	t_time			l_e;
	pthread_t		philo;
	sem_t       	*read_meals;
	t_all			*all;
}	t_philo;

int		ft_atoi(char *str);
void	ft_check_death(t_philo *philos);
int		ft_create(t_philo *philos, t_all *init);
void	ft_check_meals(t_philo *philos, t_index i);
long	ft_get_time(t_time last);
void	*routine(void *p);
void	ft_sleep(long time);
void	ft_supervisor(t_philo *philos, t_index i);
void	ft_print_philo(t_philo *philos, char *str);

#endif