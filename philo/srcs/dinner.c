/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:37:09 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:44:45 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo_thread(t_philo *philo);

void	stagger_start(t_philo *philo)
{
	if (philo->table->philosopher_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			philo_think(philo, true);
	}
}

void	philo_eat(t_philo *philo)
{
	mutex_op(&philo->left_fork->mutex, MTX_LOCK);
	print_status(GRAB_LEFT_FORK, philo);
	mutex_op(&philo->right_fork->mutex, MTX_LOCK);
	print_status(GRAB_RIGHT_FORK, philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	mutex_op(&philo->philo_mutex, MTX_LOCK);
	philo->meal_count++;
	mutex_op(&philo->philo_mutex, MTX_UNLOCK);
	print_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meal_limit > 0
		&& philo->meal_count == philo->table->meal_limit)
		set_bool(&philo->philo_mutex, &philo->full, true);
	mutex_op(&philo->left_fork->mutex, MTX_UNLOCK);
	mutex_op(&philo->right_fork->mutex, MTX_UNLOCK);
}

static void	init_philo_thread(t_philo *philo)
{
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increment_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	stagger_start(philo);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	init_philo_thread(philo);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		philo_eat(philo);
		print_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		philo_think(philo, false);
	}
	return (NULL);
}

void	*single_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increment_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	mutex_op(&philo->left_fork->mutex, MTX_LOCK);
	print_status(GRAB_LEFT_FORK, philo);
	precise_usleep(philo->table->time_to_die, philo->table);
	mutex_op(&philo->left_fork->mutex, MTX_UNLOCK);
	return (NULL);
}
