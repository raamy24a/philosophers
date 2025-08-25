/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/08/25 23:05:47 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
}

int	main(int argc, char const *argv[])
{
	int			i;
	t_philo		*p;
	pthread_t	thread;

	p = malloc (sizeof (t_philo));
	i = 0;
	while (i < p->nop)
	{
		i++;
		pthread_create(&thread, NULL, philosophers, p);
	}
	return (0);
}
