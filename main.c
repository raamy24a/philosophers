/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/09/16 15:31:26 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	timems(t_philo *philo, int x)
{
	struct timeval	*m;
	struct timeval	*s;

	if (x == 0)
	{
		gettimeofday(&m, &s);
		return (s->tv_sec * 1000 + m->tv_usec / 1000 - philo->timeatstart);
	}
	else
	{
		gettimeofday(&m, &s);
		return (s->tv_sec * 1000 + m->tv_usec / 1000);
	}
}

int	eat(t_philo *p)
{
	int	time;
	int	timeeating;

	timeeating = 0;
	time = timems(p, 0);
	p->timelasteaten = time;
	printf("%d %d is eating \n", time, p->philo_number);
	while (timems(p, 0) - p->timelasteaten < p->ttd && timeeating < p->tte)
		timeeating = time - timems(p, 0);
	if (p->timelasteaten > p->ttd)
	{
		printf("%d %d died\n", timems(p, 0), p->philo_number);
		return (0);
	}
	else
		return (1);
}

int	sleep(t_philo *p)
{
	int	time;
	int	timeslept;

	timeslept = 0;
	time = timems(p, 0);
	printf("%d %d is sleeping \n", time, p->philo_number);
	while (timems(p, 0) - p->timelasteaten < p->ttd && timeslept < p->tts)
		timeslept = time - timems(p, 0);
	if (p->timelasteaten > p->ttd)
	{
		printf("%d %d died\n", timems(p, 0), p->philo_number);
		return (0);
	}
	else
		return (1);
}

void	think(t_philo *philo)
{
	int	time;

	time = timems(philo, 0);
	printf("%d %d is thinking \n", time, philo->philo_number);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
	while (1)
	{
		while (philo->table->thread_status)
			;
		philo->timeatstart = timems(p, 1);
		eat(philo);
		philo->timeeaten++;
		sleep(philo);
		think(philo);
	}
}

int	main(int argc, char const *argv[])
{
	int			i;
	t_arg		*arg;
	t_table		*t;
	pthread_t	thread;

	if (argc != 5 && argc != 6)
		return (write (1, "erreur\n", 7));
	arg->nop = ft_atoi(argv[1]);
	arg->ttd = ft_atoi(argv[2]);
	arg->tte = ft_atoi(argv[3]);
	arg->tts = ft_atoi(argv[4]);
	if (argc == 5)
		arg->notme = 2147483647;
	if (argc == 6)
		arg->notme = ft_atoi(argv[5]);
	i = -1;
	t = malloc (sizeof (t_table *));
	t->p = malloc (sizeof(t_philo *) * arg->nop);
	i = 0;
	while (++i < arg->nop + 1)
	{
		t->p[i - 1]->philo_number = i;
		t->p[i - 1]->nop = arg->nop;
		t->p[i - 1]->ttd = arg->ttd;
		t->p[i - 1]->tte = arg->tte;
		t->p[i - 1]->tts = arg->tts;
		t->p[i - 1]->notme = arg->notme;
		t->p[i - 1]->table = t;
		if (phtread_mutex_init(t->mutex[i - 1]) != 0)
			return (printf("mutex error"));
	}
	t->thread_status = 1;
	i = -1;
	while (++i < arg->nop)
		pthread_create(&thread, NULL, philosophers, t->p[i]);
	t->thread_status = 0;
	return (0);
}
