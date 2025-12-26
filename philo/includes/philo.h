/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:39:23 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:42:55 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# define SECOND		0
# define MILLISECOND	1
# define MICROSECOND	2

typedef int				t_timecode;
typedef pthread_mutex_t	t_mutex;
typedef struct s_table	t_table;

typedef enum e_mutex_op
{
	MTX_LOCK,
	MTX_UNLOCK,
	MTX_INIT,
	MTX_DESTROY,
}	t_mutex_op;

typedef enum e_thread_op
{
	THR_CREATE,
	THR_JOIN,
	THR_DETACH,
}	t_thread_op;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	GRAB_LEFT_FORK,
	GRAB_RIGHT_FORK,
	DIED,
}	t_philo_status;

typedef struct s_fork
{
	t_mutex	mutex;
	int		fork_id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	long		meal_count;
	bool		full;
	long		last_meal_time;
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id;
	t_mutex		philo_mutex;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	bool		all_threads_ready;
	bool		end_simulation;
	t_fork		*forks;
	long		meal_limit;
	pthread_t	monitor;
	long		philosopher_count;
	t_philo		*philos;
	long		running_thread_count;
	long		start_simulation;
	t_mutex		table_mutex;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	t_mutex		write_mutex;
}	t_table;

/* main.c */
void	dinner_start(t_table *table);

/* dinner.c */
void	philo_eat(t_philo *philo);
void	*philo_routine(void *data);
void	*single_philo_routine(void *arg);
void	stagger_start(t_philo *philo);

/* monitor.c */
void	*monitor_dinner(void *data);

/* init.c */
void	philo_think(t_philo *philo, bool pre_simulation);
void	table_cleanup(t_table *table);
void	table_init(t_table *table);

/* parsing.c */
void	parse_input(t_table *table, char **av);

/* time.c */
long	get_time(t_timecode timecode);
void	precise_usleep(long usec, t_table *table);
void	print_status(t_philo_status status, t_philo *philo);

/* sync.c */
bool	all_threads_running(t_mutex *mutex, long *thread_count,
			long philosopher_count);
void	error_exit(const char *error);
void	increment_long(t_mutex *mutex, long *value);
bool	simulation_finished(t_table *table);
void	wait_all_threads(t_table *table);

/* safe_wrappers.c */
void	mutex_op(t_mutex *mutex, t_mutex_op op);
void	*safe_malloc(size_t bytes);
void	thread_op(pthread_t *thread, void *(*start_routine)(void *),
			void *data, t_thread_op op);

/* sync_access.c */
bool	get_bool(t_mutex *mutex, bool *value);
long	get_long(t_mutex *mutex, long *value);
void	set_bool(t_mutex *mutex, bool *dest, bool value);
void	set_long(t_mutex *mutex, long *dest, long value);

#endif
