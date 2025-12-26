/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:38:01 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:38:05 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo);
static bool	check_all_full(t_table *table);

static void	announce_death(t_table *table, t_philo *philo)
{
	print_status(DIED, philo);
	set_bool(&table->table_mutex, &table->end_simulation, true);
}

static bool	check_all_full(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosopher_count)
	{
		if (!get_bool(&table->philos[i].philo_mutex, &table->philos[i].full))
			return (false);
		i++;
	}
	return (true);
}

void	*monitor_dinner(void *data)
{
	int		i;
	t_table	*table;

	table = (t_table *)data;
	while (!all_threads_running(&table->table_mutex,
			&table->running_thread_count, table->philosopher_count))
		usleep(100);
	while (!simulation_finished(table))
	{
		i = 0;
		while (i < table->philosopher_count)
		{
			if (philo_died(&table->philos[i]))
				announce_death(table, &table->philos[i]);
			i++;
		}
		if (table->meal_limit > 0 && check_all_full(table))
			set_bool(&table->table_mutex, &table->end_simulation, true);
		usleep(100);
	}
	return (NULL);
}

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die_ms;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	pthread_mutex_lock(&philo->philo_mutex);
	elapsed = get_time(MILLISECOND) - (philo->last_meal_time);
	pthread_mutex_unlock(&philo->philo_mutex);
	time_to_die_ms = (philo->table->time_to_die) / 1000;
	if (elapsed > time_to_die_ms)
		return (true);
	return (false);
}
