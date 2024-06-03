/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:11:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/31 12:30:13 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_extend2(t_phil *phil, int id, int first_fork, int second_fork)
{
	pthread_mutex_lock(&phil->t_m);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), \
		pthread_mutex_unlock(&phil->t_m), -1);
	phil->is_eating[id] = true;
	phil->time_since_last_meal[id] = get_time();
	pthread_mutex_unlock(&phil->t_m);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), -1);
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), \
		pthread_mutex_unlock(&phil->print_m), -1);
	my_printf(chrono(phil->time), id, "is eating\n");
	pthread_mutex_unlock(&phil->print_m);
	pthread_mutex_lock(&phil->t_m);
	return (0);
}

int	fork_extend1(t_phil *phil, int first_fork, int second_fork, int id)
{
	pthread_mutex_lock(&phil->f_m[first_fork]);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), -1);
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), \
		pthread_mutex_unlock(&phil->print_m), -1);
	my_printf(chrono(phil->time), id, "has taken a fork\n");
	pthread_mutex_unlock(&phil->print_m);
	pthread_mutex_lock(&phil->f_m[second_fork]);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[second_fork]), \
		pthread_mutex_unlock(&phil->f_m[first_fork]), -1);
	pthread_mutex_lock(&phil->print_m);
	if (supervisor(phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), \
		pthread_mutex_unlock(&phil->print_m), -1);
	my_printf(chrono(phil->time), id, "has taken a fork\n");
	pthread_mutex_unlock(&phil->print_m);
	return (0);
}

int	take_fork(t_phil *phil, int id)
{
	int	first_fork;
	int	second_fork;
	int	tmp;

	first_fork = id;
	second_fork = (id + 1) % phil->nbr_phil;
	if (first_fork > second_fork)
	{
		tmp = second_fork;
		second_fork = first_fork;
		first_fork = tmp;
	}
	if (fork_extend1(phil, first_fork, second_fork, id) == -1)
		return (-1);
	if (fork_extend2(phil, id, first_fork, second_fork))
		return (-1);
	phil->time_since_last_meal[id] = get_time();
	pthread_mutex_unlock(&phil->t_m);
	if (ft_sleep(phil->time_to_eat / 1000, phil, id) == -1)
		return (pthread_mutex_unlock(&phil->f_m[first_fork]), \
		pthread_mutex_unlock(&phil->f_m[second_fork]), -1);
	phil->is_eating[id] = false;
	pthread_mutex_unlock(&phil->f_m[first_fork]);
	pthread_mutex_unlock(&phil->f_m[second_fork]);
	return (0);
}

void	ft_start(t_phil *phil, int *id)
{
	pthread_mutex_lock(&phil->mutex);
	*id = phil->counter;
	phil->counter++;
	pthread_mutex_unlock(&phil->mutex);
	if (*id % 2 == 1)
		ft_sleep(100, phil, *id);
	pthread_mutex_lock(&phil->start);
	phil->time_since_last_meal[*id] = get_time();
	pthread_mutex_unlock(&phil->start);
	pthread_mutex_lock(&phil->mutex);
	pthread_mutex_unlock(&phil->mutex);
}

int	supervisor(t_phil *phil, int id)
{
	pthread_mutex_lock(&phil->alive_check);
	if (phil->alive == false)
	{
		pthread_mutex_unlock(&phil->alive_check);
		return (-1);
	}
	if ((phil->time_since_last_meal[id] + (phil->time_before_death / 1000)) \
		* 1000 < get_time() * 1000)
	{
		phil->alive = false;
		pthread_mutex_unlock(&phil->alive_check);
		pthread_mutex_lock(&phil->print_m);
		my_printf(chrono(phil->time), id, "died\n");
		pthread_mutex_unlock(&phil->print_m);
		return (-1);
	}
	pthread_mutex_unlock(&phil->alive_check);
	return (0);
}
