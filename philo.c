/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:18:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/26 16:24:57 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	is_someone_died(t_data *data)
{
	int	state;

	pthread_mutex_lock(&data->someone_died_mutex);
	state = data->someone_died;
	pthread_mutex_unlock(&data->someone_died_mutex);
	return (state);
}

int	get_simulation_state(t_data *data)
{
	int	state;


	if(data->number_of_philosophers == 1)
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

void	*philo_life(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (get_simulation_state(ph->data) != 1)
		;
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
		return (NULL);
	}
	while (1)
	{
		if (ph->id % 2 == 0)
		{
			pthread_mutex_lock(ph->right_fork);
			print_action(ph, "has taken a fork");
			pthread_mutex_lock(ph->left_fork);
			print_action(ph, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(ph->left_fork);
			print_action(ph, "has taken a fork");
			pthread_mutex_lock(ph->right_fork);
			print_action(ph, "has taken a fork");
		}
		pthread_mutex_lock(&ph->last_meal_mutex);
		ph->last_meal = get_time_ms();
		pthread_mutex_unlock(&ph->last_meal_mutex);
		print_action(ph, "is eating");
		smart_sleep(ph->data->time_to_eat);
		if (ph->id % 2 == 0)
		{
			pthread_mutex_unlock(ph->right_fork);
			pthread_mutex_unlock(ph->left_fork);
		}
		else
		{
			pthread_mutex_unlock(ph->left_fork);
			pthread_mutex_unlock(ph->right_fork);
		}
		if (is_someone_died(ph->data))
			return (NULL);
		pthread_mutex_lock(&ph->eat_count_mutex);
		ph->eat_count++;
		pthread_mutex_unlock(&ph->eat_count_mutex);
		if (ph->data->must_eat > 0 && ph->eat_count >= ph->data->must_eat)
			break ;
		print_action(ph, "is sleeping");
		smart_sleep(ph->data->time_to_sleep);
		if (is_someone_died(ph->data))
			return (NULL);
		print_action(ph, "is thinking");
		if ((ph->data->time_to_die - (ph->data->time_to_eat + ph->data->time_to_sleep)) / 2 != 0)
			smart_sleep((ph->data->time_to_die - (ph->data->time_to_eat + ph->data->time_to_sleep)) / 2);//bunu araştır zaman kaymması oluyor
		if (is_someone_died(ph->data))
			return (NULL);
	}
	return (NULL);
}

void	*monitor(void *arg)
{

	t_data	*data;
	int		i;
	int		full;
	t_philo	*ph;
	long	time_since_meal;

	data = (t_data *)arg;
	smart_sleep(100);
	pthread_mutex_lock(&data->start_lock);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->start_lock);
	data->start_time = get_time_ms();
	while (1)
	{
		i = 0;
		full = 0;
		while (i < data->number_of_philosophers)
		{
			ph = &data->philos[i];
			pthread_mutex_lock(&data->someone_died_mutex);
			pthread_mutex_lock(&ph->last_meal_mutex);
			time_since_meal = get_time_ms() - ph->last_meal;
			pthread_mutex_unlock(&ph->last_meal_mutex);
			if (!data->someone_died && time_since_meal >= data->time_to_die)
			{
				data->someone_died = 1;
				printf("%ld %d died\n", get_time_ms() - data->start_time,
					ph->id);
				pthread_mutex_unlock(&data->someone_died_mutex);
				return (NULL);
			}
			pthread_mutex_lock(&ph->eat_count_mutex);
			if (data->must_eat > 0 && ph->eat_count >= data->must_eat)
				full++;
			pthread_mutex_unlock(&ph->eat_count_mutex);
			pthread_mutex_unlock(&data->someone_died_mutex);
			i++;
		}
		if (data->must_eat > 0 && full == data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->someone_died_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->someone_died_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
