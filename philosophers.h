/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 20:50:36 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/10 16:53:19 by yobenali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef int	index;

typedef struct s_all
{
	int				nb_p;
	int				tt_d;
	int				tt_e;
	int				tt_s;
	int				e_t;
	int				check;
	int				flag;
	long			simul;
	pthread_mutex_t	check_eat;
	pthread_mutex_t	r_flag;
	pthread_mutex_t	printing;
}	t_all;

typedef struct s_philo
{
	pthread_t		philo;
	long			l_e;
	int				n_e;
	int				index;
	pthread_mutex_t	read_meals;
	pthread_mutex_t	fork;
	pthread_mutex_t	*nxt_fork;	
	t_all			*all;
}	t_philo;

#endif