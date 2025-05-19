/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:05:11 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/01 01:23:24 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <string.h>

long get_time_ms(void)
{
    struct timeval tv;
    
    gettimeofday(&tv,NULL);
    return((tv.tv_sec *1000) + (tv.tv_usec / 1000));
}

void smart_sleep(long time)
{
    long start = get_time_ms();
    while ((get_time_ms() - start) < time)
        usleep(100);
}

void print_action(t_philo *philo, char *action)
{
    long time;

    pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->someone_died_mutex);
    time = get_time_ms() - philo->data->start_time;
    if(!philo->data->someone_died) {
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
	pthread_mutex_unlock(&philo->data->someone_died_mutex);
    pthread_mutex_unlock(&philo->data->print_mutex);
}
