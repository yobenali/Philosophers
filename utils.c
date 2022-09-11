/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilis.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 17:46:24 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/10 17:52:58 by yobenali         ###   ########.fr       */
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
