/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:11:37 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/01 01:19:47 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int ft_atoi(const char *str)
{
    int res = 0;
	int sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
    while(*str >= '0' && *str <= '9')
        res = (res * 10) + (*str++ - '0');
    return(res * sign);
}

int is_valid_number(char *str)
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

int init_data(t_data *data, int ac, char **av)
{
    data->number_of_philosophers = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);   
    data->time_to_eat = ft_atoi(av[3]);   
    data->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
	    data->must_eat = ft_atoi(av[5]);
    else
	    data->must_eat = -1;
    data->someone_died = 0;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
    data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
    if(!data->forks || !data->philos)
        return(1);

    for (int i = 0; i < data->number_of_philosophers; i++)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philos[i].eat_count_mutex, NULL);
	}
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->someone_died_mutex, NULL);
	return (0);
}

void	start_simulation(t_data *data)
{
	int i;

	data->start_time = get_time_ms();
	for (i = 0; i < data->number_of_philosophers; i++)
	{
		t_philo *ph = &data->philos[i];
		ph->id = i + 1;
		ph->eat_count = 0;
		ph->last_meal = data->start_time;
		ph->data = data;
		ph->left_fork = &data->forks[i];
		ph->right_fork = &data->forks[(i + 1) % data->number_of_philosophers];
		pthread_create(&ph->thread, NULL, &philo_life, ph);
		usleep(100);
	}
	
	// Monitor thread başlat
	pthread_t monitor_thread;
	if (data->number_of_philosophers > 1)
	{
		pthread_create(&monitor_thread, NULL, &monitor, data);
		pthread_join(monitor_thread, NULL); // Ölüm olunca beklemeyi bitir
	}
}


void	join_and_destroy(t_data *data)
{
	for (int i = 0; i < data->number_of_philosophers; i++)
		pthread_join(data->philos[i].thread, NULL);
	for (int i = 0; i < data->number_of_philosophers; i++)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].eat_count_mutex);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->someone_died_mutex);
	free(data->philos);
	free(data->forks);
}

int main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (printf("Error: wrong number of arguments\n"), 1);

	for (int i = 1; i < argc; i++)
	{
		if (!is_valid_number(argv[i]))
			return (printf("Error: invalid arguments\n"), 1);
	}

	if (init_data(&data, argc, argv))
		return (printf("Error: memory allocation failed\n"), 1);
	start_simulation(&data);
	join_and_destroy(&data);
	return (0);
}
