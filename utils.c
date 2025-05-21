/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:05:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/21 12:45:07 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <string.h>
#include <time.h>

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long time)
{
	struct timespec	req;
	long			start;
	long			remaining;

	start = get_time_ms();
	remaining = time - (get_time_ms() - start);
	while (remaining > 0)
	{
		req.tv_sec = 0;
		req.tv_nsec = remaining * 1000000;
		nanosleep(&req, NULL);
		remaining = time - (get_time_ms() - start);
	}
}

void	print_action(t_philo *philo, char *action)
{
	long	time;

	pthread_mutex_lock(&philo->data->someone_died_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->someone_died)
	{
		time = get_time_ms() - philo->data->start_time;
		if (strcmp(action, "is thinking") == 0)
			printf("%ld %d \033[34mis thinking\033[0m\n", time, philo->id);
		else if (strcmp(action, "has taken a fork") == 0)
			printf("%ld %d \033[32mhas taken a fork\033[0m\n", time, philo->id);
		else if (strcmp(action, "is eating") == 0)
			printf("%ld %d \033[33mis eating\033[0m\n", time, philo->id);
		else if (strcmp(action, "is sleeping") == 0)
			printf("%ld %d \033[35mis sleeping\033[0m\n", time, philo->id);
		else
			printf("%ld %d %s\n", time, philo->id, action);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->someone_died_mutex);
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
