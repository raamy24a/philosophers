/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/09/16 20:02:47 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	timems(t_table *t, int x)
{
	struct timeval	tv;

	if (x == 0)
	{
		gettimeofday(&tv, NULL);
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - t->timeatstart);
	}
	else
	{
		gettimeofday(&tv, NULL);
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	}
}

int	eat(t_philo *p)
{
	int	time;
	int	timeeating;

	time = timems(p->table, 0);
	p->timelasteaten = time;
	if (p->philo_number % 2 == 0)
	{
		pthread_mutex_lock(p->table->mutex[p->philo_number - 2]);
		pthread_mutex_lock(p->table->mutex[p->philo_number - 1]);
	}
	else
	{
		pthread_mutex_lock(p->table->mutex[p->philo_number - 1]);
		pthread_mutex_lock(p->table->mutex[p->philo_number]);
	}
	printf("%d %d is eating \n", time, p->philo_number);
	while (timems(p->table, 0) - p->timelasteaten < p->ttd && timeeating < p->tte)
		timeeating = timems(p->table, 0) - time;
	pthread_mutex_unlock(p->table->mutex[p->philo_number - 1 + p->philo_number % 2]);
	pthread_mutex_unlock(p->table->mutex[p->philo_number - 2 + p->philo_number % 2]);
	if (p->philo_number % 2 == 0)
		p->table->even_ate_last = 1;
	else
		p->table->even_ate_last = 0;
	if (p->timelasteaten > p->ttd)
	{
		printf("%d %d died\n", timems(p->table, 0), p->philo_number);
		return (0);
	}
	else
		return (1);
}

int	sleep_philo(t_philo *p)
{
	int	time;
	int	timeslept;

	timeslept = 0;
	time = timems(p->table, 0);
	printf("%d %d is sleeping \n", time, p->philo_number);
	while (timems(p->table, 0) - p->timelasteaten < p->ttd && timeslept < p->tts)
		timeslept = time - timems(p->table, 0);
	if (p->timelasteaten > p->ttd)
	{
		printf("%d %d died\n", timems(p->table, 0), p->philo_number);
		return (0);
	}
	else
		return (1);
}

void	think(t_philo *p)
{
	int	time;

	time = timems(p->table, 0);
	printf("%d %d is thinking \n", time, p->philo_number);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
	while (1)
	{
		while (philo->table->thread_status)
			usleep(1);
		eat(philo);
		philo->timeeaten++;
		sleep_philo(philo);
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
		return (printf ("erreur\n"));
	arg = malloc (sizeof (t_arg));
	arg->nop = ft_atoi(argv[1], 1, 0, 0);
	arg->ttd = ft_atoi(argv[2], 1, 0, 0);
	arg->tte = ft_atoi(argv[3], 1, 0, 0);
	arg->tts = ft_atoi(argv[4], 1, 0, 0);
	if (argc == 5)
		arg->notme = 2147483647;
	if (argc == 6)
		arg->notme = ft_atoi(argv[5], 1, 0, 0);
	t = malloc (sizeof (t_table));
	t->p = malloc (sizeof(t_philo *) * arg->nop);
	t->mutex = malloc (sizeof (pthread_mutex_t *) * arg->nop);
	i = 0;
	t->thread_status = 1;
	while (++i < arg->nop + 1)
	{
		t->p[i - 1] = malloc (sizeof(t_philo));
		t->p[i - 1]->philo_number = i;
		t->p[i - 1]->nop = arg->nop;
		t->p[i - 1]->ttd = arg->ttd;
		t->p[i - 1]->tte = arg->tte;
		t->p[i - 1]->tts = arg->tts;
		t->p[i - 1]->notme = arg->notme;
		t->p[i - 1]->table = t;
		t->mutex[i - 1] = malloc (sizeof(pthread_mutex_t));
		if (pthread_mutex_init(t->mutex[i - 1], NULL) != 0)
			return (printf("mutex error"));
	}
	i = -1;
	while (++i < arg->nop)
		pthread_create(&thread, NULL, philosophers, t->p[i]);
	t->thread_status = 0;
	t->timeatstart = timems(t, 1);
	while (1)
		;
	return (0);
}
