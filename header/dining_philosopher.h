/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dining_philosopher.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:20:53 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/14 22:20:55 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DINING_PHILOSOPHER_H
# define DINING_PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_dining_table	t_dining_table;

typedef struct s_thinker
{
	int				position;
	int				meals_count;
	long long		time_of_last_meal;
	pthread_mutex_t	*first_utensil;
	pthread_mutex_t	*second_utensil;
	pthread_mutex_t	personal_mutex;
	t_dining_table	*dining_table;
	pthread_t		life_thread;
}	t_thinker;

typedef struct s_dining_table
{
	int				thinker_count;
	int				starvation_time;
	int				feeding_duration;
	int				rest_duration;
	int				required_meals;
	int				total_fed;
	int				dinner_in_progress;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	output_lock;
	pthread_mutex_t	meal_counter_lock;
	long long		dinner_start_time;
	t_thinker		*thinkers;
	pthread_mutex_t	*utensils;
}	t_dining_table;

/* Core utility functions */
int			convert_str_to_int(const char *str);
void		release_resources(t_dining_table *table);
long long	get_current_timestamp(void);
void		display_message(t_dining_table *table, int thinker_id,
				char *action);
long long	calculate_elapsed_time(t_dining_table *table);
int			string_compare(const char *s1, const char *s2);
void		precise_sleep(long long milliseconds, t_dining_table *table);
int			validate_numeric_args(char **argv, int argc);

/* Initialization functions */
int			setup_dining_simulation(t_dining_table *table, int count,
				char **args, int arg_count);
int			setup_table_parameters(t_dining_table *table, int count,
				char **args, int arg_count);
int			setup_synchronization_objects(t_dining_table *table);
int			allocate_utensils(t_dining_table *table, int count);
int			setup_thinkers(t_dining_table *table, int count);

/* Simulation control functions */
int			launch_simulation_threads(t_dining_table *table);
void		*monitor_simulation(void *arg);
void		*thinker_lifecycle(void *arg);
int			is_simulation_active(t_dining_table *table);
int			check_for_starvation(t_dining_table *table, int pos,
				long long current);
int			check_meal_completion(t_dining_table *table, int count);

/* Philosopher actions */
void		handle_lone_thinker(t_dining_table *table, t_thinker *thinker);
void		acquire_utensils(t_thinker *thinker, t_dining_table *table);
void		perform_thinker_actions(t_thinker *thinker, t_dining_table *table);

#endif