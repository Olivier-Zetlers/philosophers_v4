/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_wrappers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:38:26 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:38:29 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	report_mutex_error(int status, t_mutex_op op);

bool	mutex_op(t_mutex *mutex, t_mutex_op op)
{
	if (op == MTX_LOCK)
		return (report_mutex_error(pthread_mutex_lock(mutex), op));
	else if (op == MTX_UNLOCK)
		return (report_mutex_error(pthread_mutex_unlock(mutex), op));
	else if (op == MTX_INIT)
		return (report_mutex_error(pthread_mutex_init(mutex, NULL), op));
	else if (op == MTX_DESTROY)
		return (report_mutex_error(pthread_mutex_destroy(mutex), op));
	print_error("error: invalid mutex opcode");
	return (false);
}

static bool	report_mutex_error(int status, t_mutex_op op)
{
	if (status == 0)
		return (true);
	if (status == EINVAL && (op == MTX_LOCK || op == MTX_UNLOCK))
		print_error("error: mutex is invalid");
	else if (status == EINVAL && op == MTX_INIT)
		print_error("error: mutex attributes invalid");
	else if (status == EDEADLK)
		print_error("error: mutex deadlock detected");
	else if (status == EPERM)
		print_error("error: mutex not owned by current thread");
	else if (status == ENOMEM)
		print_error("error: insufficient memory for mutex");
	else if (status == EBUSY)
		print_error("error: mutex is already locked");
	return (false);
}

static bool	report_thread_error(int status, t_thread_op op)
{
	if (status == 0)
		return (true);
	else if (status == EAGAIN)
		print_error("error: insufficient resources for thread");
	else if (status == EPERM)
		print_error("error: thread operation not permitted");
	else if (status == EINVAL && op == THR_CREATE)
		print_error("error: invalid thread attributes");
	else if (status == EINVAL && (op == THR_JOIN || op == THR_DETACH))
		print_error("error: invalid thread identifier");
	else
		print_error("error: thread operation failed");
	return (false);
}

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		print_error("error: malloc failed");
	return (ret);
}

bool	thread_op(pthread_t *thread, void *(*start_routine)(void *),
		void *data, t_thread_op op)
{
	if (op == THR_CREATE)
		return (report_thread_error(pthread_create(thread, NULL,
					start_routine, data), op));
	else if (op == THR_JOIN)
		return (report_thread_error(pthread_join(*thread, NULL), op));
	else if (op == THR_DETACH)
		return (report_thread_error(pthread_detach(*thread), op));
	print_error("error: invalid thread opcode (use CREATE, JOIN, DETACH)");
	return (false);
}
