/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:38:38 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:38:41 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	mutex_op(mutex, MTX_LOCK);
	ret = *value;
	mutex_op(mutex, MTX_UNLOCK);
	return (ret);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	mutex_op(mutex, MTX_LOCK);
	ret = *value;
	mutex_op(mutex, MTX_UNLOCK);
	return (ret);
}

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	mutex_op(mutex, MTX_LOCK);
	*dest = value;
	mutex_op(mutex, MTX_UNLOCK);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	mutex_op(mutex, MTX_LOCK);
	*dest = value;
	mutex_op(mutex, MTX_UNLOCK);
}
