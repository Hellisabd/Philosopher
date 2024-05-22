/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:11:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/23 00:37:14 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

ssize_t chrono(ssize_t time)
{
	struct timeval tv;
	ssize_t	actual_time;

	gettimeofday(&tv, NULL);
	actual_time = (tv.tv_usec - time) / 100000;
	return (actual_time);
}

void	take_fork(t_phil *phil, int id)
{
	int first_fork;
	int second_fork;
	int	tmp;

	first_fork = id;
	second_fork = (id + 1) % phil->nbr_phil;
	if (first_fork > second_fork)
	{
		tmp = second_fork;
		second_fork = first_fork;
		first_fork = tmp;
	}
	pthread_mutex_lock(&phil->f_m[first_fork]);
	printf("%ld %d has taken a fork\n", chrono(phil->time), id);
	pthread_mutex_lock(&phil->f_m[second_fork]);
	printf("%ld %d has taken a fork\n", chrono(phil->time), id);
	pthread_mutex_lock(&phil->print_m);
	pthread_mutex_lock(&phil->t_m);
	phil->time_since_last_meal[id] = get_current_time();
	phil->is_eating[id] = true;
	pthread_mutex_unlock(&phil->t_m);
	printf("%ld %d is eating\n", chrono(phil->time), id);
	pthread_mutex_unlock(&phil->print_m);
	usleep(phil->time_to_eat);
	phil->is_eating[id] = false;
	pthread_mutex_unlock(&phil->f_m[first_fork]);
	pthread_mutex_unlock(&phil->f_m[second_fork]);
}

void ft_start(t_phil *phil, int id)
{
	pthread_mutex_lock(&phil->start);
	pthread_mutex_lock(&phil->t_m);
	phil->time_since_last_meal[id] = get_current_time();
	pthread_mutex_unlock(&phil->t_m);
	pthread_mutex_unlock(&phil->start);
}

void	*supervisor(void	*data)
{
	t_phil	*phil;
	int		id;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->mutex);
	id = phil->counter;
	pthread_mutex_unlock(&phil->mutex);
	while (1)
	{
		pthread_mutex_lock(&phil->t_m);
		pthread_mutex_lock(&phil->alive_check);
		if (phil->time_since_last_meal[id] < get_current_time() - phil->time_before_death && phil->is_eating[id] == false)
		{
			phil->alive = false;
			pthread_mutex_unlock(&phil->alive_check);
			return (NULL);	
		}
		pthread_mutex_unlock(&phil->alive_check);
		printf("id : %d\nlast_eat : %ld\ndeadline : %ld\n", id, phil->time_since_last_meal[id], get_current_time() - phil->time_before_death);
		pthread_mutex_unlock(&phil->t_m);
	}
	return (NULL);
}

void	*routine(void *data)
{
	t_phil	*phil;
	int		id;
	pthread_t	id_s;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->mutex);
	id = phil->counter;
	pthread_mutex_unlock(&phil->mutex);
	ft_start(phil, id);
	pthread_mutex_lock(&phil->mutex);
	pthread_create(&id_s, NULL, &supervisor, phil);
	phil->counter++;
	pthread_mutex_unlock(&phil->mutex);
	while (1)
	{
		pthread_mutex_lock(&phil->alive_check);
		if (phil->alive == false)
		{
			pthread_mutex_unlock(&phil->alive_check);
			break ;
		}
		pthread_mutex_unlock(&phil->alive_check);
		take_fork(phil, id);
		usleep(phil->time_to_sleep);
	}
	pthread_join(id_s, NULL);
	return (NULL);
}
