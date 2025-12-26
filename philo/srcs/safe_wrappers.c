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

static void	report_mutex_error(int status, t_mutex_op op);

void	mutex_op(t_mutex *mutex, t_mutex_op op)
{
	if (op == MTX_LOCK)
		report_mutex_error(pthread_mutex_lock(mutex), op);
	else if (op == MTX_UNLOCK)
		report_mutex_error(pthread_mutex_unlock(mutex), op);
	else if (op == MTX_INIT)
		report_mutex_error(pthread_mutex_init(mutex, NULL), op);
	else if (op == MTX_DESTROY)
		report_mutex_error(pthread_mutex_destroy(mutex), op);
	else
		error_exit("error: invalid mutex opcode");
}

static void	report_mutex_error(int status, t_mutex_op op)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (op == MTX_LOCK || op == MTX_UNLOCK))
		error_exit("error: mutex is invalid");
	else if (status == EINVAL && op == MTX_INIT)
		error_exit("error: mutex attributes invalid");
	else if (status == EDEADLK)
		error_exit("error: mutex deadlock detected");
	else if (status == EPERM)
		error_exit("error: mutex not owned by current thread");
	else if (status == ENOMEM)
		error_exit("error: insufficient memory for mutex");
	else if (status == EBUSY)
		error_exit("error: mutex is already locked");
}

static void	report_thread_error(int status, t_thread_op op)
{
	if (status == 0)
		return ;
	else if (status == EAGAIN)
		error_exit("error: insufficient resources for thread");
	else if (status == EPERM)
		error_exit("error: thread operation not permitted");
	else if (status == EINVAL && op == THR_CREATE)
		error_exit("error: invalid thread attributes");
	else if (status == EINVAL && (op == THR_JOIN || op == THR_DETACH))
		error_exit("error: invalid thread identifier");
	else
		error_exit("error: thread operation failed");
}

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		error_exit("error: malloc failed");
	return (ret);
}

void	thread_op(pthread_t *thread, void *(*start_routine)(void *),
		void *data, t_thread_op op)
{
	if (op == THR_CREATE)
		report_thread_error(pthread_create(thread, NULL,
				start_routine, data), op);
	else if (op == THR_JOIN)
		report_thread_error(pthread_join(*thread, NULL), op);
	else if (op == THR_DETACH)
		report_thread_error(pthread_detach(*thread), op);
	else
		error_exit("error: invalid thread opcode (use CREATE, JOIN, DETACH)");
}
