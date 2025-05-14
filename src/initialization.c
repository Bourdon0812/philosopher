/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:48 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:21:04 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

int	setup_table_parameters(t_dining_table *table, int count,
							char **args, int arg_count)
{
	table->thinker_count = count;
	table->starvation_time = convert_str_to_int(args[2]);
	table->feeding_duration = convert_str_to_int(args[3]);
	table->rest_duration = convert_str_to_int(args[4]);
	table->total_fed = 0;
	table->dinner_in_progress = 1;
	if (arg_count == 6)
	{
		table->required_meals = convert_str_to_int(args[5]);
		if (table->required_meals <= 0)
		{
			write(1, "Invalid meal requirement\n", 25);
			return (-1);
		}
	}
	else
		table->required_meals = -1;
	return (0);
}

int	setup_synchronization_objects(t_dining_table *table)
{
	if (pthread_mutex_init(&table->meal_counter_lock, NULL) != 0
		|| pthread_mutex_init(&table->state_lock, NULL) != 0
		|| pthread_mutex_init(&table->output_lock, NULL) != 0)
	{
		printf("Mutex initialization error");
		return (-1);
	}
	return (0);
}

int	allocate_utensils(t_dining_table *table, int count)
{
	int	i;

	table->utensils = malloc(sizeof(pthread_mutex_t) * count);
	if (!table->utensils)
	{
		printf("Failed to allocate memory for utensils");
		return (-1);
	}
	i = 0;
	while (i < count)
	{
		if (pthread_mutex_init(&table->utensils[i], NULL) != 0)
		{
			printf("Failed to initialize utensil mutex");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	setup_thinkers(t_dining_table *table, int count)
{
	int				i;
	struct timeval	tv;

	table->thinkers = malloc(sizeof(t_thinker) * count);
	if (!table->thinkers)
	{
		printf("Failed to allocate memory for thinkers");
		free(table->utensils);
		return (-1);
	}
	gettimeofday(&tv, NULL);
	table->dinner_start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	i = 0;
	while (i < count)
	{
		table->thinkers[i].position = i + 1;
		table->thinkers[i].meals_count = 0;
		table->thinkers[i].time_of_last_meal = get_current_timestamp();
		table->thinkers[i].first_utensil = &table->utensils[i];
		table->thinkers[i].second_utensil = &table->utensils[(i + 1) % count];
		table->thinkers[i].dining_table = table;
		pthread_mutex_init(&table->thinkers[i].personal_mutex, NULL);
		i++;
	}
	return (0);
}

int	setup_dining_simulation(t_dining_table *table, int count,
							char **args, int arg_count)
{
	if (setup_table_parameters(table, count, args, arg_count) != 0)
		return (-1);
	if (setup_synchronization_objects(table) != 0)
		return (-1);
	if (allocate_utensils(table, count) != 0)
		return (-1);
	if (setup_thinkers(table, count) != 0)
		return (-1);
	return (0);
}
