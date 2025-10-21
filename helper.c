/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:16:11 by radib             #+#    #+#             */
/*   Updated: 2025/10/21 03:35:48 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(const char *nptr, int sign, int total, int i)
{
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		total *= 10;
		total += nptr[i] - '0';
		if ((total * sign > 0) && sign == -1)
			return (-20000000000);
		if ((total < 0) && sign == 1)
			return (-20000000000);
		i++;
	}
	if (!(nptr[i]) && !(sign == -1 && total == 0 && i == 1))
		return ((long)(total * sign));
	return (-20000000000);
}

void	prnt_s(char *s, unsigned long long time, int philo, t_table *t)
{
	if (createandcheck(2, t) == 1)
		printf("%lld %d %s\n", time, philo, s);
}

int	createandcheck(int x, t_table *t)
{
	if (x == 0)
		t->everyone_is_alive = 1;
	if (x == 1)
	{
		pthread_mutex_lock(t->checkallowed);
		t->everyone_is_alive = 0;
		pthread_mutex_unlock(t->checkallowed);
	}
	if (x == 2)
	{
		pthread_mutex_lock(t->checkallowed);
		if (t->everyone_is_alive == 0)
			x = 3;
		pthread_mutex_unlock(t->checkallowed);
	}
	if (x == 3)
		return (-1);
	return (1);
}
