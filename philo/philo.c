/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:18:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 01:00:16 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>
#include <sys/time.h>
#include "unistd.h"

int	is_someone_died(t_data *data)
{
	int	state;

	pthread_mutex_lock(&data->someone_died_mutex);
	state = data->someone_died;
	pthread_mutex_unlock(&data->someone_died_mutex);
	return (state);
}

static int	get_simulation_state(t_data *data)
{
	int	state;

	if (data->number_of_philosophers == 1)
	{
		data->start_time = get_time_ms();
		state = 1;
	}
	else
	{
		pthread_mutex_lock(&data->start_lock);
		state = data->start_flag;
		pthread_mutex_unlock(&data->start_lock);
	}
	return (state);
}

int	philo_start(t_philo *ph)
{
	while (get_simulation_state(ph->data) != 1)
		usleep(1);
	pthread_mutex_lock(&ph->last_meal_mutex);
	ph->last_meal = get_time_ms();
	pthread_mutex_unlock(&ph->last_meal_mutex);
	if (ph->id % 2 == 0)
		smart_sleep(1);
	if (ph->data->number_of_philosophers == 1)
	{
		pthread_mutex_lock(ph->left_fork);
		print_action(ph, "has taken a fork");
		smart_sleep(ph->data->time_to_die);
		pthread_mutex_lock(&ph->data->someone_died_mutex);
		ph->data->someone_died = 1;
		printf("%ld %d died\n", get_time_ms() - ph->data->start_time, ph->id);
		pthread_mutex_unlock(&ph->data->someone_died_mutex);
		pthread_mutex_unlock(ph->left_fork);
		return (1);
	}
	return (0);
}
