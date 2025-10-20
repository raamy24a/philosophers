/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:26:37 by radib             #+#    #+#             */
/*   Updated: 2025/10/20 16:44:19 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>

typedef struct arg
{
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					notme;
	int					philo_created;
}				t_arg;

typedef struct t	t_table;

typedef struct p
{
	int					pnbr;
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					timesincelasteat;
	int					timelasteaten;
	int					timeeaten;
	int					notme;
	pthread_mutex_t		*check;
	t_table				*table;
}				t_philo;

typedef struct t
{
	t_philo				**p;
	pthread_mutex_t		**mutex;
	pthread_mutex_t		*checkallowed;
	int					everyone_is_alive;
	int					thread_status;
	long long			timeatstart;
	pthread_t			*thread;

}				t_table;

long	ft_atoi(const char *nptr, int sign, int total, int i);
int		createandcheck(int x, t_table *t);
void	prnt_s(char *s, unsigned long long time, long philo, t_table *t);

#endif