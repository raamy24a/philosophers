/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/09/17 04:05:21 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	timems(t_table *t)
{
	struct timeval	tv;

		gettimeofday(&tv, NULL);
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - t->timeatstart);
}
int unlocktwo(t_philo *p, int x)
{
	if (x == 2)
	{
	pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
	pthread_mutex_unlock(p->table->mutex[p->pnbr]);
	return (x);
	}
	if (x == 1)
	{
	pthread_mutex_unlock(p->table->mutex[p->pnbr]);
	pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
	return (x);
	}
	if (x == 0)
	{
	pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
	pthread_mutex_unlock(p->table->mutex[0]);
	return (x);
	}
	return (-1);
}
int locktwo(t_philo *p, int x)
{
	if (x == 2)
	{
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	pthread_mutex_lock(p->table->mutex[p->pnbr]);
	return (x);
	}
	if (x == 1)
	{
	pthread_mutex_lock(p->table->mutex[p->pnbr]);
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	return (x);
	}
	if (x == 0)
	{
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	pthread_mutex_lock(p->table->mutex[0]);
	return (x);
	}
	return (-1);
}
int	eat(t_philo *p)
{
	int	time;
	int	x;
	int	timeeating;

	if (p->pnbr == p->nop)
		x = locktwo(p, 0);
	else if (p->pnbr % 2 == 1)
		x = locktwo(p, 1);
	else if (p->pnbr % 2 == 0)
		x = locktwo(p, 2);
	time = timems(p->table);
	p->timelasteaten = time;
	timeeating = 0;
	printf("%d %d is eating \n", time, p->pnbr);
	while (timems(p->table) - p->timelasteaten < p->ttd && timeeating < p->tte)
		timeeating = timems(p->table) - time;
	unlocktwo(p, x);
	if (timems(p->table) - p->timelasteaten > p->ttd)
	{
		p->table->everyone_is_alive = 0;
		return (printf("%d %d died\n", timems(p->table), p->pnbr));
	}
	return (1);
}

int	sleep_philo(t_philo *p)
{
	int	time;
	int	timeslept;

	timeslept = 0;
	time = timems(p->table);
	printf("%d %d is sleeping \n", time, p->pnbr);
	while (timems(p->table) - p->timelasteaten < p->ttd && timeslept < p->tts)
		timeslept = timems(p->table) - time;
	if (timems(p->table) - p->timelasteaten > p->ttd)
	{
		printf("%d %d died\n", timems(p->table), p->pnbr);
		p->table->everyone_is_alive = 0;
		return (0);
	}
	else
		return (1);
}

void	think(t_philo *p)
{
	int	time;

	time = timems(p->table);
	printf("%d %d is thinking \n", time, p->pnbr);
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
		if (philo->table->everyone_is_alive == 0)
			return (NULL);
		philo->timeeaten++;
		sleep_philo(philo);
			if (philo->table->everyone_is_alive == 0)
		return (NULL);
		think(philo);
			if (philo->table->everyone_is_alive == 0)
		return (NULL);
	}
}

int	main(int argc, char const *argv[])
{
	int			i;
	t_arg		*arg;
	t_table		*t;
	pthread_t	*thread;

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
	thread = malloc (sizeof(pthread_t *) * arg->nop);
	t->p = malloc (sizeof(t_philo *) * arg->nop);
	t->mutex = malloc (sizeof (pthread_mutex_t *) * arg->nop);
	i = 0;
	t->thread_status = 1;
	while (++i < arg->nop + 1)
	{
		t->p[i - 1] = malloc (sizeof(t_philo));
		t->p[i - 1]->pnbr = i;
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
		pthread_create(&thread[i], NULL, philosophers, t->p[i]);
	t->thread_status = 0;
	t->timeatstart = 0;
	t->timeatstart = timems(t);
	t->everyone_is_alive = 1;
	while (t->everyone_is_alive)
	;
	i = 0;
	while (++i < arg->nop + 1)
	{
		free (t->p[i - 1]);
		pthread_mutex_destroy(t->mutex[i - 1]);
		free (t->mutex[i - 1]);
		pthread_join(thread[i - 1], NULL);
	}
	free (t->mutex);
	free (t->p);
	free (t);
	free (arg);
	free (thread);
	return (0);
}
