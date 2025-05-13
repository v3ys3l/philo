/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:18:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/12 20:19:44 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void *philo_life(void *arg)
{
	t_philo *ph = (t_philo *)arg;

	if (ph->id % 2 == 0)
		usleep(100);

	while (1)
	{
		print_action(ph, "is thinking");

		pthread_mutex_lock(ph->left_fork);
		print_action(ph, "has taken a fork");

		pthread_mutex_lock(ph->right_fork);
		print_action(ph, "has taken a fork");

		pthread_mutex_lock(&ph->data->print_mutex);
		ph->last_meal = get_time_ms();
		pthread_mutex_unlock(&ph->data->print_mutex);

		print_action(ph, "is eating");
		smart_sleep(ph->data->time_to_eat);

		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);

		ph->eat_count++;
		if (ph->data->must_eat > 0 && ph->eat_count >= ph->data->must_eat)
			break;

		print_action(ph, "is sleeping");
		smart_sleep(ph->data->time_to_sleep);
	}
	return NULL;
}


void *monitor(void *arg)
{
	t_data *data = (t_data *)arg;
	int i;
	int full;

	while (1)
	{
		i = 0;
		full = 0;
		while (i < data->number_of_philosophers)
		{
			t_philo *ph = &data->philos[i];

			pthread_mutex_lock(&data->print_mutex);
			if (!data->someone_died &&
				(get_time_ms() - ph->last_meal) >= data->time_to_die)
			{
				data->someone_died = 1;
				printf("%ld %d died\n", get_time_ms() - data->start_time, ph->id);
				pthread_mutex_unlock(&data->print_mutex);
				return NULL;
			}
			if (data->must_eat > 0 && ph->eat_count >= data->must_eat)
				full++;
			pthread_mutex_unlock(&data->print_mutex);
			i++;
		}
		if (data->must_eat > 0 && full == data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->print_mutex);
			data->someone_died = 1; // Simülasyonu durdurmak için
			pthread_mutex_unlock(&data->print_mutex);
			return NULL;
		}
		usleep(1000);
	}
	return NULL;
}
