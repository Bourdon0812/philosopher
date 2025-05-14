/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinker_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:36 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:24:06 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

int	is_simulation_active(t_dining_table *table)
{
	pthread_mutex_lock(&table->state_lock);
	if (!table->dinner_in_progress)
	{
		pthread_mutex_unlock(&table->state_lock);
		return (0);
	}
	pthread_mutex_unlock(&table->state_lock);
	return (1);
}

void	handle_lone_thinker(t_dining_table *table, t_thinker *thinker)
{
	display_message(table, thinker->position, "has taken a utensil");
	usleep(table->starvation_time * 1000);
}

void	acquire_utensils(t_thinker *thinker, t_dining_table *table)
{
	if (thinker->position % 2 == 0)
	{
		pthread_mutex_lock(thinker->second_utensil);
		display_message(table, thinker->position, "has taken a fork");
		pthread_mutex_lock(thinker->first_utensil);
		display_message(table, thinker->position, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(thinker->first_utensil);
		display_message(table, thinker->position, "has taken a fork");
		pthread_mutex_lock(thinker->second_utensil);
		display_message(table, thinker->position, "has taken a fork");
	}
}

void	perform_thinker_actions(t_thinker *thinker, t_dining_table *table)
{
	pthread_mutex_lock(&thinker->personal_mutex);
	thinker->time_of_last_meal = get_current_timestamp();
	display_message(table, thinker->position, "is eating");
	if (table->required_meals != -1)
		thinker->meals_count++;
	pthread_mutex_unlock(&thinker->personal_mutex);
	precise_sleep(table->feeding_duration, table);
	pthread_mutex_unlock(thinker->first_utensil);
	pthread_mutex_unlock(thinker->second_utensil);
	display_message(table, thinker->position, "is sleeping");
	precise_sleep(table->rest_duration, table);
	display_message(table, thinker->position, "is thinking");
}

void	*thinker_lifecycle(void *arg)
{
	t_thinker		*thinker;
	t_dining_table	*table;

	thinker = (t_thinker *)arg;
	table = thinker->dining_table;
	if (thinker->position % 2 == 0)
		usleep(1000);
	if (table->thinker_count == 1)
	{
		handle_lone_thinker(table, thinker);
		return (NULL);
	}
	while (1)
	{
		if (!is_simulation_active(table))
			break ;
		acquire_utensils(thinker, table);
		perform_thinker_actions(thinker, table);
		if (!is_simulation_active(table))
			break ;
	}
	return (NULL);
}
