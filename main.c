/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:20:16 by radib             #+#    #+#             */
/*   Updated: 2025/09/19 04:13:03 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	timems(t_table *t)
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
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	pthread_mutex_unlock(p->table->mutex[p->pnbr]);
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr,p->pnbr);
	return (x);
	}
	if (x == 1)
	{
	pthread_mutex_unlock(p->table->mutex[p->pnbr]);
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr,p->pnbr);
	pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	return (x);
	}
	if (x == 0)
	{
	pthread_mutex_unlock(p->table->mutex[p->pnbr - 1]);
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	pthread_mutex_unlock(p->table->mutex[0]);
	printf("%lld %d has freed a fork %d\n", timems(p->table), p->pnbr, 0 );
	return (x);
	}
	return (-1);
}
int locktwo(t_philo *p, int x)
{
	if (x == 2)
	{
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr,p->pnbr);
	pthread_mutex_lock(p->table->mutex[p->pnbr]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr, p->pnbr);
	return (x);
	}
	if (x == 1)
	{
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr,p->pnbr);
	pthread_mutex_lock(p->table->mutex[p->pnbr]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr, p->pnbr);
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1);
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr, p->pnbr - 1);
	return (x);
	}
	if (x == 0)
	{
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr,p->pnbr - 1 );
	pthread_mutex_lock(p->table->mutex[p->pnbr - 1]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr, p->pnbr - 1);
	printf("%lld %d tried to fork %d\n", timems(p->table), p->pnbr, 0 );
	pthread_mutex_lock(p->table->mutex[0]);
	printf("%lld %d has taken a fork %d\n", timems(p->table), p->pnbr, 0);
	return (x);
	}
	return (x);
}
int thereenoughtimetoeat(t_philo *p)
{
	printf("%d\n", p->tte + p->table->p[p->pnbr - 1]->timelasteaten);
	if (p->pnbr != 1)
	{
		if (p->tte + p->table->p[p->pnbr - 1]->timelasteaten > p->ttd - p->timelasteaten - timems(p->table))
		{
			printf("a\n");
			usleep(1000 * (p->ttd - (timems(p->table) - p->timelasteaten)));
			return (0);
		}
	}
	else if (p->pnbr == 1)
		if (p->tte - timems(p->table) + p->table->p[p->pnbr - 1]->timelasteaten > p->ttd - p->timelasteaten - timems(p->table))
		{
			printf("a\n");
			usleep(1000 * (p->ttd - (timems(p->table) - p->timelasteaten)));
			return (0);
		}
	return (1);
}
int	eat(t_philo *p)
{
	int	time;
	int	x;
	int	timeeating;

	if (thereenoughtimetoeat(p) == 0)
		return (-1);
	printf("%d %d\n", p->pnbr, p->nop);
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
	while (timeeating < p->tte)
		timeeating = timems(p->table) - time;
	if (p->table->everyone_is_alive == 0)
		return (0);
	unlocktwo(p, x);
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
	printf("%d has appeared\n",philo->pnbr );
	while (1)
	{
		while (philo->table->thread_status)
			usleep(1000);
		if (philo->pnbr % 2 == 1)
			while (philo->table->p[1]->timeeaten == 0)
				usleep(100);
		if (philo->table->everyone_is_alive == 0)
		{
			printf("%d has disappeared\n",philo->pnbr );
			return (NULL);
		}
		if (eat(philo) == -1)
			philo->table->everyone_is_alive = 0;
		if (philo->table->everyone_is_alive == 0)
		{
			printf("%d has disappeared\n",philo->pnbr );
			return (NULL);
		}
		philo->timeeaten++;
		sleep_philo(philo);
		if (philo->table->everyone_is_alive == 0)
		{
			printf("%d has disappeared\n",philo->pnbr );
			return (NULL);
		}
		think(philo);
		if (philo->table->everyone_is_alive == 0)
		{
			printf("%d has disappeared\n",philo->pnbr );
			return (NULL);
		}
	}
	printf("%d has disappeared\n",philo->pnbr );
	return (NULL);
}
void *watchers(void *table)
{
	t_table *t;
	int 	x;
	int		someonedied;
	
	someonedied = 0;
	usleep (100);
	t = (t_table *) table;
	while (1)
	{
		x = 0;
		while (x < t->p[0]->nop)
		{
			if (timems(t) - t->p[x]->timelasteaten >= t->p[x]->ttd)
				someonedied = 1;
			if (someonedied == 1)
				break ;
			x++;
		}
			if (someonedied == 1)
				break ;
		usleep(1000);
	}
	printf("%lld %d %lld diedwaa\n", timems(t), t->p[x]->pnbr, timems(t) - t->p[x]->timelasteaten);
	t->everyone_is_alive = 0;
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
		t->p[i - 1]->table = t;
		t->p[i - 1]->timelasteaten = 0;
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
	t->everyone_is_alive = 1;
	t->thread_status = 0;
	pthread_create(&thread[i], NULL, watchers, t);
	while (t->everyone_is_alive == 1)
		usleep(9000);
	i = -1;
	usleep(9000);
	while (++i < arg->nop)
	{
		free (t->p[i]);
		pthread_mutex_destroy(t->mutex[i]);
		free (t->mutex[i]);
		pthread_join(thread[i], NULL);
	}
	pthread_join(thread[i], NULL);
	free (t->mutex);
	free (t->p);
	free (t);
	free (arg);
	free (thread);
	return (0);
}
