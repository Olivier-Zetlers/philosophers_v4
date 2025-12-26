/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:38:49 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:38:53 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	all_threads_running(t_mutex *mutex, long *thread_count,
		long philosopher_count)
{
	bool	ret;

	ret = false;
	mutex_op(mutex, MTX_LOCK);
	if (*thread_count == philosopher_count)
		ret = true;
	mutex_op(mutex, MTX_UNLOCK);
	return (ret);
}

void	error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	increment_long(t_mutex *mutex, long *value)
{
	mutex_op(mutex, MTX_LOCK);
	(*value)++;
	mutex_op(mutex, MTX_UNLOCK);
}

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}

void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		usleep(100);
}
