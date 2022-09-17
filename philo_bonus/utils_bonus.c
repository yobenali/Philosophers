/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yobenali <yobenali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 21:20:07 by yobenali          #+#    #+#             */
/*   Updated: 2022/09/17 23:21:13 by yobenali         ###   ########.fr       */
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
