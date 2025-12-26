/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozetlers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 03:38:13 by ozetlers          #+#    #+#             */
/*   Updated: 2025/12/26 03:38:16 by ozetlers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool			is_digit(char c);
static const char	*skip_whitespace_and_sign(const char *str, bool *error);
static const char	*validate_input(const char *str, bool *error);

static long	ft_atoi(const char *str, bool *error)
{
	long	num;
	int		digit;

	num = 0;
	str = validate_input(str, error);
	if (*error)
		return (0);
	while (is_digit(*str))
	{
		digit = *str - '0';
		if (num > (INT_MAX - digit) / 10)
		{
			print_error("error: value exceeds INT_MAX");
			*error = true;
			return (0);
		}
		num = num * 10 + digit;
		str++;
	}
	return (num);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*skip_whitespace_and_sign(const char *str, bool *error)
{
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		print_error("error: negative values not allowed");
		*error = true;
	}
	return (str);
}

bool	parse_input(t_table *table, char **av)
{
	long	min_time_us;
	bool	error;

	error = false;
	min_time_us = 60000;
	table->philosopher_count = ft_atoi(av[1], &error);
	table->time_to_die = ft_atoi(av[2], &error) * 1000;
	table->time_to_eat = ft_atoi(av[3], &error) * 1000;
	table->time_to_sleep = ft_atoi(av[4], &error) * 1000;
	if (error)
		return (false);
	if (table->time_to_die < min_time_us || table->time_to_eat < min_time_us
		|| table->time_to_sleep < min_time_us)
	{
		print_error("error: time values must be at least 60 ms");
		return (false);
	}
	if (av[5])
		table->meal_limit = ft_atoi(av[5], &error);
	else
		table->meal_limit = -1;
	return (!error);
}

static const char	*validate_input(const char *str, bool *error)
{
	const char	*start;
	size_t		len;

	len = 0;
	str = skip_whitespace_and_sign(str, error);
	if (*error)
		return (str);
	if (!is_digit(*str))
		return (print_error("error: invalid character, expected digit"),
			*error = true, str);
	start = str;
	while (is_digit(*str) && ++len)
		str++;
	if (len > 10)
		return (print_error("error: value exceeds INT_MAX"),
			*error = true, start);
	while (is_space(*str))
		str++;
	if (*str != '\0')
		return (print_error("error: invalid character, expected digit"),
			*error = true, start);
	return (start);
}
