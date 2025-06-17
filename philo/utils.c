/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:05:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 00:57:53 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long time)
{
	long	start;

	start = get_time_ms() + time;
	while (get_time_ms() < start)
		usleep(250);
}

static int	ft_strcmp(char *dest, char *src)
{
	int	i;

	i = 0;
	while (dest[i] && src[i] && dest[i] == src[i])
		i++;
	return ((unsigned char)dest[i] - (unsigned char)src[i]);
}

void	print_action(t_philo *philo, char *action)
{
	long	time;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_someone_died(philo->data))
	{
		time = get_time_ms() - philo->data->start_time;
		if (ft_strcmp(action, "is thinking") == 0)
			printf("%ld %d \033[34mis thinking\033[0m\n", time, philo->id);
		else if (ft_strcmp(action, "has taken a fork") == 0)
			printf("%ld %d \033[32mhas taken a fork\033[0m\n", time, philo->id);
		else if (ft_strcmp(action, "is eating") == 0)
			printf("%ld %d \033[33mis eating\033[0m\n", time, philo->id);
		else if (ft_strcmp(action, "is sleeping") == 0)
			printf("%ld %d \033[35mis sleeping\033[0m\n", time, philo->id);
		else
			printf("%ld %d %s\n", time, philo->id, action);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		res = (res * 10) + (*str++ - '0');
	return (res * sign);
}
