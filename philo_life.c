/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:20:41 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/31 19:29:00 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "utils.h"

static void	philo_take_forks(t_philo *ph)
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
}

static void	philo_eat_and_release(t_philo *ph)
{
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
}

static int	philo_sleep_think(t_philo *ph)
{
	long	wait_time;

	if (is_someone_died(ph->data))
		return (1);
	pthread_mutex_lock(&ph->eat_count_mutex);
	ph->eat_count++;
	pthread_mutex_unlock(&ph->eat_count_mutex);
	if (ph->data->must_eat > 0 && ph->eat_count >= ph->data->must_eat)
		return (1);
	print_action(ph, "is sleeping");
	smart_sleep(ph->data->time_to_sleep);
	if (is_someone_died(ph->data))
		return (1);
	print_action(ph, "is thinking");
	wait_time = (ph->data->time_to_die - (ph->data->time_to_eat
				+ ph->data->time_to_sleep)) / 2;
	if (wait_time > 0)
		smart_sleep(wait_time);
	if (is_someone_died(ph->data))
		return (1);
	return (0);
}

void	*philo_life(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (philo_start(ph))
		return (NULL);
	while (1)
	{
		philo_take_forks(ph);
		philo_eat_and_release(ph);
		if (philo_sleep_think(ph))
			break ;
	}
	return (NULL);
}
