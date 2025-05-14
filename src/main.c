/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:45 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:21:07 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/dining_philosopher.h"

int	validate_arguments(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (convert_str_to_int(argv[i]) <= 0)
			return (-1);
		i++;
	}
	return (0);
}

void	launch_threads(t_dining_table *table)
{
	int			i;
	int			j;
	pthread_t	monitor_thread;

	i = -1;
	while (++i < table->thinker_count)
	{
		if (pthread_create(&table->thinkers[i].life_thread, NULL,
				thinker_lifecycle, &table->thinkers[i]) != 0)
		{
			printf("Failed to create thread for thinker %d\n", i + 1);
			pthread_mutex_lock(&table->state_lock);
			table->dinner_in_progress = 0;
			pthread_mutex_unlock(&table->state_lock);
			j = -1;
			while (++j < i)
				pthread_join(table->thinkers[j].life_thread, NULL);
			return ;
		}
	}
	if (pthread_create(&monitor_thread, NULL, monitor_simulation, table) != 0)
		printf("Failed to create monitoring thread\n");
	pthread_join(monitor_thread, NULL);
}

int	prepare_simulation(t_dining_table *table, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		write(1, "Incorrect argument count\n", 25);
		return (-1);
	}
	if (validate_arguments(argv) == -1)
	{
		write(1, "Invalid argument values\n", 24);
		return (-1);
	}
	if (setup_dining_simulation(table, convert_str_to_int(argv[1]),
			argv, argc) == -1)
	{
		write(1, "Failed to initialize simulation\n", 32);
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_dining_table	table;
	int				i;

	if (validate_numeric_args(argv, argc) == 1)
	{
		printf("Arguments must be numeric values\n");
		return (-1);
	}
	if (prepare_simulation(&table, argc, argv) == -1)
		return (-1);
	launch_threads(&table);
	i = 0;
	while (i < table.thinker_count)
	{
		if (pthread_join(table.thinkers[i].life_thread, NULL) != 0)
			printf("Failed to join thread %d\n", i + 1);
		i++;
	}
	release_resources(&table);
	return (0);
}
