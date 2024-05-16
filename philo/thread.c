/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:11:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/16 13:42:54 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *data)
{
	t_phil	*phil;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->mutex);
	printf("I'm thread : %lu\n", phil->id[phil->counter]);
	phil->counter++;
	printf("I'm the counter : %d\n", phil->counter);
	printf("I'm the i : %d\n", phil->i);
	pthread_mutex_unlock(&phil->mutex);
	return (NULL);
}