/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/10/20 16:55:25 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	timems(t_table *t)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - t->timeatstart);
}

int	unlocktwo(t_philo *p, int x)
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
		pthread_mutex_unlock(p->table->mutex[0]);
		pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
		return (x);
	}
	return (-1);
}

int	locktwo(t_philo *p, int x)
{
	if (x == 2)
	{
		pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		pthread_mutex_lock(p->table->mutex[p->pnbr]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		return (x);
	}
	if (x == 1)
	{
		pthread_mutex_lock(p->table->mutex[p->pnbr]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		return (x);
	}
	if (x == 0)
	{
		pthread_mutex_lock(p->table->mutex[0]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
		prnt_s("has taken a fork", timems(p->table), p->pnbr, p->table);
		return (x);
	}
	return (x);
}

int	eat(t_philo *p, int x, int timeeating, long long time)
{
	if (p->pnbr == p->nop)
		x = locktwo(p, 0);
	else if (p->pnbr % 2 == 1)
		x = locktwo(p, 1);
	else if (p->pnbr % 2 == 0)
		x = locktwo(p, 2);
	time = timems(p->table);
	p->timelasteaten = time;
	timeeating = 0;
	prnt_s("is eating", time, p->pnbr, p->table);
	while (timeeating < p->tte && createandcheck(2, p->table) == 1)
	{
		timeeating = timems(p->table) - time;
		usleep(1000);
	}
	if (createandcheck(2, p->table) == -1)
		return (-1);
	unlocktwo(p, x);
	return (1);
}

int	sleep_philo(t_philo *p)
{
	long long	time;
	int			timeslept;

	timeslept = 0;
	time = timems(p->table);
	prnt_s("is sleeping", time, p->pnbr, p->table);
	while (timems(p->table) - p->timelasteaten < p->ttd && timeslept < p->tts)
	{
		timeslept = timems(p->table) - time;
		usleep(1000);
	}
	return (1);
}

void	think(t_philo *p)
{
	long long	time;

	time = timems(p->table);
	prnt_s("is thinking", time, p->pnbr, p->table);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *) p;
	while (1)
	{
		while (philo->table->thread_status)
			usleep(1000);
		if (philo->pnbr % 2 == 1)
			while (philo->table->p[1]->timeeaten == 0)
				usleep(1000);
		if (createandcheck(2, philo->table) == -1)
			return (NULL);
		if (eat(philo, 0, 0, (long long) 0) == -1)
		{
			createandcheck(1, philo->table);
			return (NULL);
		}
		philo->timeeaten++;
		sleep_philo(philo);
		think(philo);
		return (NULL);
	}
}

int	everyone_ate_enough(t_table *t)
{
	int	i;

	i = 0;
	pthread_mutex_lock(t->checkallowed);
	while (i < t->p[0]->nop)
	{
		if (timems(t) - t->p[i]->timelasteaten >= t->p[i]->ttd)
			return (1);
		i++;
	}
	pthread_mutex_unlock(t->checkallowed);
}

int check_death(int x, t_table *t)
{
	pthread_mutex_lock(t->p[x]->check);
	if (timems(t) - t->p[x]->timelasteaten >= t->p[x]->ttd)
		return (1);
	pthread_mutex_unlock(t->p[x]->check);
	return (0);
}

void *watchers(void *table)
{
	t_table	*t;
	int		x;

	t = (t_table *) table;
	usleep (1000 * t->p[0]->ttd);
	while (1)
	{
		x = 0;
		while (x < t->p[0]->nop)
		{
			if (check_death(x, t))
				createandcheck(1, table);
			if (createandcheck(2, t) == -1)
			{
				printf("%lld %d  died\n", timems(t), t->p[x]->pnbr);
				break ;
			}
			x++;
		}
		if (createandcheck(2, t) == -1)
			break ;
		usleep(1000);
	}
	return (NULL);
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
	thread = malloc (sizeof(pthread_t) * (arg->nop + 1));
	t->p = malloc (sizeof(t_philo *) * arg->nop);
	t->mutex = malloc(sizeof(pthread_mutex_t *) * arg->nop);
	i = 0;
	t->thread = thread;
	while (++i < arg->nop + 1)
	{
		t->p[i - 1] = malloc (sizeof(t_philo));
		t->p[i - 1]->pnbr = i;
		t->p[i - 1]->nop = arg->nop;
		t->p[i - 1]->ttd = arg->ttd;
		t->p[i - 1]->tte = arg->tte;
		t->p[i - 1]->tts = arg->tts;
		t->p[i - 1]->notme = arg->notme;
		t->p[i - 1]->timeeaten = 0;
		t->p[i - 1]->table = t;
		t->p[i - 1]->timelasteaten = 0;
		if (pthread_mutex_init(t->p[i - 1]->check, NULL) != 0)
			return (printf("mutex error\n"));
		t->mutex[i - 1] = malloc (sizeof(pthread_mutex_t));
		if (pthread_mutex_init(t->mutex[i - 1], NULL) != 0)
			return (printf("mutex error\n"));
	}
	t->thread_status = 1;
	i = -1;
	while (++i < arg->nop)
	{
		pthread_create(&thread[i], NULL, philosophers, t->p[i]);
		usleep(100);
	}
	t->timeatstart = 0;
	t->timeatstart = timems(t);
	createandcheck(0, t);
	t->thread_status = 0;
	pthread_create(&thread[i], NULL, watchers, t);
	while (createandcheck(2, t) != -1)
		usleep(1000000);
	i = -1;
	while (++i < arg->nop)
	{
		pthread_join(thread[i], NULL);
		free (t->p[i]);
		pthread_mutex_destroy(t->mutex[i]);
		free (t->mutex[i]);
	}
	pthread_join(thread[i], NULL);
	free (t->mutex);
	free (t->p);
	free (t->checkallowed);
	free (t);
	free (arg);
	free (thread);
	return (0);
}
