/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:03:25 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/12 00:48:45 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "philo.h"

long	get_time_ms(void);
void	smart_sleep(long time);
void	print_action(t_philo *philo, char *action);
int		ft_atoi(const char *str);

#endif