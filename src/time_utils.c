/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:31 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:21:15 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

void	precise_sleep(long long milliseconds, t_dining_table *table)
{
	long long	start_time;
	long long	current_time;

	start_time = calculate_elapsed_time(table);
	while (1)
	{
		pthread_mutex_lock(&table->state_lock);
		if (!table->dinner_in_progress)
		{
			pthread_mutex_unlock(&table->state_lock);
			break ;
		}
		pthread_mutex_unlock(&table->state_lock);
		current_time = calculate_elapsed_time(table);
		if ((current_time - start_time) >= milliseconds)
			break ;
		usleep(500);
	}
}

long long	get_current_timestamp(void)
{
	struct timeval	tv;
	long long		milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
	return (milliseconds);
}

long long	calculate_elapsed_time(t_dining_table *table)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000)
		- table->dinner_start_time);
}
