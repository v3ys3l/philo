/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:59:41 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/01 01:21:11 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_data t_data;

typedef struct s_philo {
    int             id;
    int             eat_count;
    pthread_mutex_t eat_count_mutex;
    long            last_meal;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data          *data;
} t_philo;

struct s_data {
    int             number_of_philosophers;
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             must_eat;

    long            start_time;
    int             someone_died;

    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
	pthread_mutex_t someone_died_mutex;

    t_philo         *philos;
};


void *monitor(void *arg);
void *philo_life(void *arg);

#endif
