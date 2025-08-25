/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:26:37 by radib             #+#    #+#             */
/*   Updated: 2025/08/25 23:08:23 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <pthread.h>

typedef struct p
{
	int					nop;
	int					ttd;
	int					tte;
	int					tts;
	int					ntme;
	pthread_mutex_t		mutex;
}				t_philo;

#endif