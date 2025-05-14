/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:41 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:21:09 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

int	check_for_starvation(t_dining_table *table, int pos,
							long long current_time)
{
	long long	time_since_last_meal;
	long long	last_meal_time;

	pthread_mutex_lock(&table->thinkers[pos].personal_mutex);
	last_meal_time = table->thinkers[pos].time_of_last_meal;
	pthread_mutex_unlock(&table->thinkers[pos].personal_mutex);
	time_since_last_meal = current_time - last_meal_time;
	if (time_since_last_meal > table->starvation_time)
	{
		pthread_mutex_lock(&table->state_lock);
		table->dinner_in_progress = 0;
		pthread_mutex_unlock(&table->state_lock);
		display_message(table, table->thinkers[pos].position, "died");
		return (1);
	}
	return (0);
}

int	check_meal_completion(t_dining_table *table, int count)
{
	int	i;
	int	all_satisfied;

	all_satisfied = 1;
	i = 0;
	while (i < count)
	{
		pthread_mutex_lock(&table->thinkers[i].personal_mutex);
		if (table->required_meals != -1
			&& table->thinkers[i].meals_count < table->required_meals)
			all_satisfied = 0;
		pthread_mutex_unlock(&table->thinkers[i].personal_mutex);
		i++;
	}
	if (table->required_meals != -1 && all_satisfied)
	{
		pthread_mutex_lock(&table->state_lock);
		table->dinner_in_progress = 0;
		pthread_mutex_unlock(&table->state_lock);
		return (1);
	}
	return (0);
}

void	*monitor_simulation(void *arg)
{
	t_dining_table	*table;
	int				i;
	long long		current_time;

	table = (t_dining_table *)arg;
	while (1)
	{
		i = 0;
		while (i < table->thinker_count)
		{
			current_time = get_current_timestamp();
			if (check_for_starvation(table, i, current_time))
				return (NULL);
			i++;
		}
		if (check_meal_completion(table, table->thinker_count))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
