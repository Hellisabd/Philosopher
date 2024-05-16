/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:11:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/16 17:30:38 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

ssize_t chrono(ssize_t time, t_phil *phil)
{
	struct timeval tv;
	ssize_t	actual_time;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&phil->t_m);
	actual_time = (tv.tv_usec - time) / 1000;
	pthread_mutex_unlock(&phil->t_m);
	return (actual_time);
}

void	take_fork(t_phil *phil, int id)
{
	if (id == phil->nbr_phil - 1)
	{
		pthread_mutex_lock(&phil->f_m[id]);
		pthread_mutex_lock(&phil->f_m[0]);
		printf("%ld %d has taken a fork\n", chrono(phil->time, phil), id);
		usleep(phil->time_to_eat);
		printf("%ld %d is eating\n", chrono(phil->time, phil), id);
		pthread_mutex_unlock(&phil->f_m[id]);
		pthread_mutex_unlock(&phil->f_m[0]);
	}
	else
	{
		pthread_mutex_lock(&phil->f_m[id]);
		pthread_mutex_lock(&phil->f_m[id + 1]);
		printf("%ld %d has taken a fork\n", chrono(phil->time, phil), id);
		usleep(phil->time_to_eat);
		printf("%ld %d is eating\n", chrono(phil->time, phil), id);
		pthread_mutex_unlock(&phil->f_m[id]);
		pthread_mutex_unlock(&phil->f_m[id + 1]);
	}
}

void ft_start(t_phil *phil)
{
	pthread_mutex_lock(&phil->start);
	pthread_mutex_unlock(&phil->start);
}

void	*routine(void *data)
{
	t_phil	*phil;
	int		id;

	phil = (t_phil *)data;
	ft_start(phil);
	pthread_mutex_lock(&phil->mutex);
	id = phil->counter;
	phil->counter++;
	pthread_mutex_unlock(&phil->mutex);
	take_fork(phil, id);
	return (NULL);
}

// void	*routine1(void *data)
// {
// 	while (personne n est mort)
// 	{
// 		take a fork;
// 		eat;
// 		drop fork;
// 		think;
// 		sleep;
// 	}
// }