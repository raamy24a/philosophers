/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:26:37 by radib             #+#    #+#             */
/*   Updated: 2025/09/16 19:52:47 by radib            ###   ########.fr       */
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
	int					philo_number;
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					timesincelasteat;
	int					timelasteaten;
	int					timeeaten;
	int					notme;
	t_table				*table;
}				t_philo;

typedef struct t
{
	t_philo				**p;
	pthread_mutex_t		**mutex;
	int					even_ate_last;
	int					thread_status;
	int					timeatstart;

}				t_table;

long	ft_atoi(const char *nptr, int sign, int total, int i);

#endif