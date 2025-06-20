/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:59:41 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 00:48:55 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					eat_count;
	long				last_meal;
	pthread_t			thread;
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		eat_count_mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_data				*data;
}						t_philo;

struct					s_data
{
	int					number_of_philosophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					must_eat;
	int					start_flag;
	long				start_time;
	int					someone_died;
	pthread_mutex_t		start_lock;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		someone_died_mutex;

	t_philo				*philos;
};

void					*monitor(void *arg);
void					*philo_life(void *arg);
int						init_data(t_data *data, int ac, char **av);

int						is_someone_died(t_data *data);
int						philo_start(t_philo *ph);
int						parse_arguments(t_data *data, int ac, char **av);
#endif
