/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/08/28 15:29:51 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
	while ();
}

int	main(int argc, char const *argv[])
{
	int			i;
	t_philo		*p;
	pthread_t	thread;

	p = malloc (sizeof (t_philo));
	if (argc != 5 || argc != 6)
		return (write (1, "erreur\n", 7));
	p->nop = ft_atoi(argv[1]);
	p->ttd = ft_atoi(argv[2]);
	p->tte = ft_atoi(argv[3]);
	p->tts = ft_atoi(argv[4]);
	p->philo_number = 1;
	if (argc == 6)
		p->notme = ft_atoi(argv[5]);
	i = -1;
	while (++i < p->nop)
	{
		pthread_create(&thread, NULL, philosophers, p);
		p->philo_number++;
	}
	return (0);
}
