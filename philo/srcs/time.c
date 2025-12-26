/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:39:01 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:39:05 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_timecode timecode)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("error: gettimeofday failed");
	if (timecode == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (timecode == MILLISECOND)
		return (tv.tv_sec * 1e3 + (tv.tv_usec / 1e3));
	else if (timecode == MICROSECOND)
		return (tv.tv_sec * 1e6 + tv.tv_usec);
	else
		error_exit("error: invalid time code");
	return (1337);
}

void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time(MICROSECOND);
	while (get_time(MICROSECOND) - start < usec)
	{
		if (simulation_finished(table))
			break ;
		elapsed = get_time(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > 1e3)
			usleep(remaining / 2);
		else
		{
			while (get_time(MICROSECOND) - start < usec)
				;
		}
	}
}

void	print_status(t_philo_status status, t_philo *philo)
{
	long	elapsed;

	if (philo->full)
		return ;
	mutex_op(&philo->table->write_mutex, MTX_LOCK);
	elapsed = get_time(MILLISECOND) - philo->table->start_simulation;
	if ((status == GRAB_LEFT_FORK || status == GRAB_RIGHT_FORK)
		&& !simulation_finished(philo->table))
		printf("%ld %d has taken a fork\n", elapsed, philo->id + 1);
	if ((status == EATING) && !simulation_finished(philo->table))
		printf("%ld %d is eating\n", elapsed, philo->id + 1);
	if ((status == SLEEPING) && !simulation_finished(philo->table))
		printf("%ld %d is sleeping\n", elapsed, philo->id + 1);
	if ((status == THINKING) && !simulation_finished(philo->table))
		printf("%ld %d is thinking\n", elapsed, philo->id + 1);
	if ((status == DIED) && !simulation_finished(philo->table))
		printf("%ld %d died\n", elapsed, philo->id + 1);
	mutex_op(&philo->table->write_mutex, MTX_UNLOCK);
}
