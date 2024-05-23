/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:11:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/23 16:56:43 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t chrono(ssize_t time)
{
	struct timeval tv;
	uint64_t	actual_time;

	gettimeofday(&tv, NULL);
	// printf("actual_time in usec : %ld | base_time in usec : %ld\n", tv.tv_usec, time);
	actual_time =  (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / 1000) - time;
	return (actual_time);
}

int	take_fork(t_phil *phil, int id)
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
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		return (-1);
	}
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		pthread_mutex_unlock(&phil->print_m);
		return (-1);
	}
	my_printf(ORANGE, chrono(phil->time), id, "has taken a fork\n");
	pthread_mutex_unlock(&phil->print_m);
	pthread_mutex_lock(&phil->f_m[second_fork]);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		return (-1);
	}
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		pthread_mutex_unlock(&phil->print_m);
		return (-1);
	}
	my_printf(ORANGE, chrono(phil->time), id, "has taken a fork\n");
	pthread_mutex_unlock(&phil->print_m);
	pthread_mutex_lock(&phil->t_m);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		pthread_mutex_unlock(&phil->t_m);
		return (-1);
	}
	phil->is_eating[id] = true;
	phil->time_since_last_meal[id] = get_time();
	pthread_mutex_unlock(&phil->t_m);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		return (-1);
	}
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		pthread_mutex_unlock(&phil->print_m);
		return (-1);
	}
	my_printf(GREEN, chrono(phil->time), id, "is eating\n");
	pthread_mutex_unlock(&phil->print_m);
	if (ft_sleep(phil->time_to_eat / 1000, phil, id))
	{
		pthread_mutex_unlock(&phil->f_m[first_fork]);
		pthread_mutex_unlock(&phil->f_m[second_fork]);
		return (-1);
	}
	phil->is_eating[id] = false;
	pthread_mutex_lock(&phil->t_m);
	phil->time_since_last_meal[id] = get_time();
	pthread_mutex_unlock(&phil->t_m);
	pthread_mutex_unlock(&phil->f_m[first_fork]);
	pthread_mutex_unlock(&phil->f_m[second_fork]);
	return (0);
}

void ft_start(t_phil *phil, int id)
{
	pthread_mutex_lock(&phil->start);
	phil->time_since_last_meal[id] = get_time();
	pthread_mutex_unlock(&phil->start);
}

int	supervisor(t_phil *phil, int id)
{
	pthread_mutex_lock(&phil->alive_check);
	if (phil->alive == false)
	{
		pthread_mutex_unlock(&phil->alive_check);
		return (-1);
	}
	if ((phil->time_since_last_meal[id] + (phil->time_before_death / 1000)) * 1000 < get_time() * 1000)
	{
		phil->alive = false;
		my_printf(RED, chrono(phil->time), id, "died\n");
		pthread_mutex_unlock(&phil->alive_check);
		return (-1);
	}
	pthread_mutex_unlock(&phil->alive_check);
	return (0);
}

void	*routine(void *data)
{
	t_phil	*phil;
	int		id;
	// pthread_t	id_s;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->mutex);
	id = phil->counter;
	phil->counter++;
	pthread_mutex_unlock(&phil->mutex);
	ft_start(phil, id);
	pthread_mutex_lock(&phil->mutex);
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
		pthread_mutex_lock(&phil->print_m);
		if (supervisor(phil, id) == -1)
		{
			pthread_mutex_unlock(&phil->print_m);
			return (NULL);
		}
		my_printf(PURPLE, chrono(phil->time), id, "is thinking\n");
		pthread_mutex_unlock(&phil->print_m);
		if (supervisor(phil, id) == -1)
			return (NULL);
		take_fork(phil, id);
		if (supervisor(phil, id) == -1)
			return (NULL);
		pthread_mutex_lock(&phil->print_m);
		if (supervisor(phil, id) == -1)
			return (pthread_mutex_unlock(&phil->print_m), NULL);
		my_printf(BLUE, chrono(phil->time), id, "is sleeping\n");
		pthread_mutex_unlock(&phil->print_m);
		if (supervisor(phil, id) == -1)
			return (NULL);
		if (ft_sleep(phil->time_to_sleep / 1000, phil, id) == -1)
			return (NULL);
		if (supervisor(phil, id) == -1)
			return (NULL);
	}
	return (NULL);
}
