/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:11:37 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 00:57:37 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static int	is_valid_number(char *str)
{
	if (*str == '-')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	parse_arguments(t_data *data, int ac, char **av)
{
	data->number_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = -1;
	if (data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || data->number_of_philosophers <= 0)
	{
		printf("ERROR: Wrong argument!\n");
		return (1);
	}
	return (0);
}

static void	start_simulation(t_data *data)
{
	int			i;
	t_philo		*ph;
	pthread_t	monitor_thread;

	data->start_flag = 0;
	data->start_time = get_time_ms();
	i = 0;
	while (i < data->number_of_philosophers)
	{
		ph = &data->philos[i];
		ph->id = i + 1;
		ph->eat_count = 0;
		ph->last_meal = data->start_time;
		ph->data = data;
		ph->left_fork = &data->forks[i];
		ph->right_fork = &data->forks[(i + 1) % data->number_of_philosophers];
		if (pthread_create(&ph->thread, NULL, &philo_life, ph))
			printf("Error: Pthread_create");
		i++;
	}
	if (data->number_of_philosophers > 1)
	{
		pthread_create(&monitor_thread, NULL, &monitor, data);
		pthread_join(monitor_thread, NULL);
	}
}

static void	join_and_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].eat_count_mutex);
		pthread_mutex_destroy(&data->philos[i].last_meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_died_mutex);
	free(data->philos);
	free(data->forks);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 5 && argc != 6)
		return (printf("Error: wrong number of arguments\n"), 1);
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (printf("Error: invalid arguments\n"), 1);
		i++;
	}
	if (init_data(&data, argc, argv))
		return (printf("Error: memory allocation failed\n"), 1);
	start_simulation(&data);
	join_and_destroy(&data);
	return (0);
}
