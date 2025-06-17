/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:18:34 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 00:57:42 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>
#include <sys/time.h>

static void	monitor_start(t_data *data)
{
	smart_sleep(100);
	pthread_mutex_lock(&data->start_lock);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->start_lock);
	data->start_time = get_time_ms();
}

static int	check_philo_death(t_data *data, t_philo *ph)
{
	long	time_since_meal;

	pthread_mutex_lock(&data->someone_died_mutex);
	pthread_mutex_lock(&ph->last_meal_mutex);
	time_since_meal = get_time_ms() - ph->last_meal;
	pthread_mutex_unlock(&ph->last_meal_mutex);
	if (!data->someone_died && time_since_meal >= data->time_to_die)
	{
		data->someone_died = 1;
		printf("%ld %d died\n", get_time_ms() - data->start_time, ph->id);
		pthread_mutex_unlock(&data->someone_died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->someone_died_mutex);
	return (0);
}

static int	check_philo_full(t_data *data, t_philo *ph)
{
	int	is_full;

	pthread_mutex_lock(&ph->eat_count_mutex);
	is_full = (data->must_eat > 0 && ph->eat_count >= data->must_eat);
	pthread_mutex_unlock(&ph->eat_count_mutex);
	return (is_full);
}

static int	check_all_philos(t_data *data)
{
	int	i;
	int	full;

	i = 0;
	full = 0;
	while (i < data->number_of_philosophers)
	{
		if (check_philo_death(data, &data->philos[i]))
			return (-1);
		if (check_philo_full(data, &data->philos[i]))
			full++;
		i++;
	}
	if (data->must_eat > 0 && full == data->number_of_philosophers)
		return (1);
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		result;

	data = (t_data *)arg;
	monitor_start(data);
	while (1)
	{
		result = check_all_philos(data);
		if (result == -1)
			return (NULL);
		if (result == 1)
		{
			pthread_mutex_lock(&data->someone_died_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->someone_died_mutex);
			return (NULL);
		}
	}
}
