/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:26:37 by radib             #+#    #+#             */
/*   Updated: 2025/09/16 03:35:25 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct arg
{
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					notme;
	int					philo_created;
}				t_arg;
typedef struct p
{
	int					philo_number;
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					timesincelasteat;
	int					timeatstart;
	int					timelasteaten;
	int					notme;
	pthread_mutex_t		mutex;
}				t_philo;

typedef struct t
{
	t_philo	**p;
}				t_table;

#endif