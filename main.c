/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/09/15 15:49:20 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	timems(t_philo *philo, int x)
{
	struct timeval	*time;

	if (x == 0)
	{
		gettimeofday(&time);
		return (time.tv_sec * 1000 + time.tv_usec / 1000 - philo->timeatstart);
	}
	else
	{
		gettimeofday(&time);
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	}
}

int	eat(t_philo *philo)
{
	int	*time;
	int	timeeating;

	timeeating = 0;
	time = timems(philo, 0);
	philo->timelastaeaten = time;
	printf("%d %d is eating \n", time, philo->philo_number);
	while (timems(philo, 0) - p->timelastaeaten < p->ttd && timeeating < p->tte)
		timeeating = time - timems(philo, 0);
	if (p->timelastaeaten > p->ttd)
	{
		printf("%d %d died\n", timems(philo, 0), philo->philo_number);
		return (0);
	}
	else
		return (1);
}

int	sleep(t_philo *philo)
{
	int	*time;
	int	timeslept;

	timeslept = 0;
	time = timems(philo, 0);
	printf("%d %d is sleeping \n", time, philo->philo_number);
	while (timems(philo, 0) - p->timelastaeaten < p->ttd && timeslept < p->tts)
		timeslept = time - timems(philo, 0);
	if (p->timelastaeaten > p->ttd)
	{
		printf("%d %d died\n", timems(philo, 0), philo->philo_number);
		return (0);
	}
	else
		return (1);
}

void	think(t_philo *philo)
{
	int	*time;

	time = timems(philo, 0);
	printf("%d %d is thinking \n", time, philo->philo_number);
}

int	checkifalivetoeat(t_philo *philo)
{
	return (0);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
	while (1)
	{
		eat(philo);
		sleep(philo);
		think(philo);
	}
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
	p->timelastaeaten = 0;
	p->timeatstart = gettimeofday(p, 1);
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
