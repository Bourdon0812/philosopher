/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:27 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:21:18 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

int	convert_str_to_int(const char *str)
{
	int	i;
	int	result;
	int	sign;

	sign = 1;
	result = 0;
	i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (result * sign);
}

void	release_resources(t_dining_table *table)
{
	int	i;

	i = 0;
	while (i < table->thinker_count)
	{
		pthread_mutex_destroy(&table->utensils[i]);
		pthread_mutex_destroy(&table->thinkers[i].personal_mutex);
		i++;
	}
	pthread_mutex_destroy(&table->state_lock);
	pthread_mutex_destroy(&table->meal_counter_lock);
	pthread_mutex_destroy(&table->output_lock);
	free(table->thinkers);
	free(table->utensils);
}

void	display_message(t_dining_table *table, int thinker_id, char *action)
{
	int	simulation_active;

	pthread_mutex_lock(&table->state_lock);
	simulation_active = table->dinner_in_progress;
	pthread_mutex_unlock(&table->state_lock);
	pthread_mutex_lock(&table->output_lock);
	if (simulation_active || string_compare(action, "died") == 0)
	{
		printf("%lld %d %s\n",
			calculate_elapsed_time(table), thinker_id, action);
	}
	pthread_mutex_unlock(&table->output_lock);
}

int	string_compare(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	validate_numeric_args(char **argv, int argc)
{
	int	i;
	int	j;

	j = 1;
	while (j < argc)
	{
		i = 0;
		while (argv[j][i])
		{
			if (argv[j][i] > '9' || argv[j][i] < '0')
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}
