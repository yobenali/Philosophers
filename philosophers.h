/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 20:50:36 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/08 22:23:42 by yobenali         ###   ########.fr       */
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

typedef struct s_all
{
	int				nb_p;
	int				tt_d;
	int				tt_e;
	int				tt_s;
	int				e_t;
	int				flag;
	long			simul;
	pthread_mutex_t	r_flag;
	pthread_mutex_t	printing;
}	t_all;

typedef struct s_philo
{
	pthread_t		philo;
	long			l_e;
	long			n_e;
	int				index;
	pthread_mutex_t	fork;
	pthread_mutex_t	read_meals;
	pthread_mutex_t	*nxt_fork;	
	t_all			*all;
}	t_philo;

#endif