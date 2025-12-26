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
	{
		print_error("error: gettimeofday failed");
		return (-1);
	}
	if (timecode == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (timecode == MILLISECOND)
		return (tv.tv_sec * 1e3 + (tv.tv_usec / 1e3));
	else if (timecode == MICROSECOND)
		return (tv.tv_sec * 1e6 + tv.tv_usec);
	print_error("error: invalid time code");
	return (-1);
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

static void	put_number(long n)
{
	char	c;

	if (n >= 10)
		put_number(n / 10);
	c = '0' + (n % 10);
	write(1, &c, 1);
}

static void	print_philo_msg(long elapsed, int id, const char *msg)
{
	put_number(elapsed);
	write(1, " ", 1);
	put_number(id);
	write(1, " ", 1);
	while (*msg)
		write(1, msg++, 1);
	write(1, "\n", 1);
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
		print_philo_msg(elapsed, philo->id + 1, "has taken a fork");
	else if ((status == EATING) && !simulation_finished(philo->table))
		print_philo_msg(elapsed, philo->id + 1, "is eating");
	else if ((status == SLEEPING) && !simulation_finished(philo->table))
		print_philo_msg(elapsed, philo->id + 1, "is sleeping");
	else if ((status == THINKING) && !simulation_finished(philo->table))
		print_philo_msg(elapsed, philo->id + 1, "is thinking");
	else if ((status == DIED) && !simulation_finished(philo->table))
		print_philo_msg(elapsed, philo->id + 1, "died");
	mutex_op(&philo->table->write_mutex, MTX_UNLOCK);
}
