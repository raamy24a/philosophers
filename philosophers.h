/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:26:37 by radib             #+#    #+#             */
/*   Updated: 2025/09/16 13:01:52 by radib            ###   ########.fr       */
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

typedef struct t	t_table;

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
	int					timeeaten;
	int					notme;
	pthread_mutex_t		mutex;
	t_table				table;
}				t_philo;

typedef struct t
{
	t_philo	**p;
	int		thread_status;

}				t_table;

#endif