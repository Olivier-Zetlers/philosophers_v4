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
static const char	*validate_input(const char *str);

static long	ft_atoi(const char *str)
{
	long	num;
	int		digit;

	num = 0;
	str = validate_input(str);
	while (is_digit(*str))
	{
		digit = *str - '0';
		if (num > (INT_MAX - digit) / 10)
			error_exit("error: value exceeds INT_MAX");
		num = num * 10 + digit;
		str++;
	}
	return (num);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

void	parse_input(t_table *table, char **av)
{
	long	min_time_us;

	min_time_us = 60000;
	table->philosopher_count = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]) * 1000;
	table->time_to_eat = ft_atoi(av[3]) * 1000;
	table->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (table->time_to_die < min_time_us || table->time_to_eat < min_time_us
		|| table->time_to_sleep < min_time_us)
		error_exit("error: time values must be at least 60 ms");
	if (av[5])
		table->meal_limit = ft_atoi(av[5]);
	else
		table->meal_limit = -1;
}

static const char	*validate_input(const char *str)
{
	const char	*start;
	size_t		len;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_exit("error: negative values not allowed");
	if (!is_digit(*str))
		error_exit("error: invalid character, expected digit");
	start = str;
	while (is_digit(*str))
	{
		len++;
		str++;
	}
	if (len > 10)
		error_exit("error: value exceeds INT_MAX");
	while (is_space(*str))
		str++;
	if (*str != '\0')
		error_exit("error: invalid character, expected digit");
	return (start);
}
