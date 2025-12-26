/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:37:29 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:37:33 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	int	philosopher_count;

	philosopher_count = philo->table->philosopher_count;
	if (philo_position % 2 != 0)
	{
		philo->left_fork = &forks[philo_position];
		philo->right_fork = &forks[(philo_position + 1) % philosopher_count];
	}
	else
	{
		philo->left_fork = &forks[(philo_position + 1) % philosopher_count];
		philo->right_fork = &forks[philo_position];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philosopher_count)
	{
		philo = table->philos + i;
		philo->id = i;
		philo->full = false;
		philo->meal_count = 0;
		philo->table = table;
		mutex_op(&philo->philo_mutex, MTX_INIT);
		assign_forks(philo, table->forks, i);
		i++;
	}
}

void	philo_think(t_philo *philo, bool pre_simulation)
{
	long	time_to_eat;
	long	time_to_sleep;
	long	think_duration;

	if (!pre_simulation)
		print_status(THINKING, philo);
	if (philo->table->philosopher_count % 2 == 0)
		return ;
	time_to_eat = philo->table->time_to_eat;
	time_to_sleep = philo->table->time_to_sleep;
	think_duration = time_to_eat * 2 - time_to_sleep;
	if (think_duration < 0)
		think_duration = 0;
	precise_usleep(think_duration * 0.42, philo->table);
}

void	table_cleanup(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < table->philosopher_count)
	{
		philo = &table->philos[i];
		mutex_op(&philo->philo_mutex, MTX_DESTROY);
		i++;
	}
	mutex_op(&table->table_mutex, MTX_DESTROY);
	mutex_op(&table->write_mutex, MTX_DESTROY);
	free(table->forks);
	free(table->philos);
}

void	table_init(t_table *table)
{
	int	i;

	i = 0;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->running_thread_count = 0;
	table->philos = safe_malloc(table->philosopher_count * sizeof(t_philo));
	table->forks = safe_malloc(table->philosopher_count * sizeof(t_fork));
	mutex_op(&table->table_mutex, MTX_INIT);
	mutex_op(&table->write_mutex, MTX_INIT);
	while (i < table->philosopher_count)
	{
		mutex_op(&table->forks[i].mutex, MTX_INIT);
		table->forks[i].fork_id = i;
		i++;
	}
	philo_init(table);
}
