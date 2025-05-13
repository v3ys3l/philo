/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:03:25 by vbicer            #+#    #+#             */
/*   Updated: 2025/05/01 01:23:33 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "philo.h"

long get_time_ms(void);
void smart_sleep(long time);
void print_action(t_philo *philo, char *action);

#endif