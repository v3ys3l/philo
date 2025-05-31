/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:22:33 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/31 19:22:48 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes_and_memory(t_data *data)
{
	int	i;

	data->someone_died = 0;
	data->forks = malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers);
	data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!data->forks || !data->philos)
		return (1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philos[i].eat_count_mutex, NULL);
		pthread_mutex_init(&data->philos[i].last_meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&data->start_lock, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->someone_died_mutex, NULL);
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	if (parse_arguments(data, ac, av))
		return (1);
	if (init_mutexes_and_memory(data))
		return (1);
	return (0);
}
