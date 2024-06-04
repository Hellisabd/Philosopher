/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:28:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/06/04 15:18:30 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	chrono(ssize_t time)
{
	struct timeval	tv;
	uint64_t		actual_time;

	gettimeofday(&tv, NULL);
	actual_time = (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / 1000) - time;
	return (actual_time);
}

int	one_phil(t_phil *phil, int id)
{
	if (phil->nbr_phil == 1)
	{
		my_printf(chrono(phil->time), id, "is thinking\n");
		my_printf(chrono(phil->time), id, "has taken a fork\n");
		ft_sleep(phil->time_before_death / 1000, phil);
		my_printf(chrono(phil->time), id, "died\n");
		return (-1);
	}
	return (0);
}

void	actualise_death(t_phil *phil, int *i)
{
	pthread_mutex_lock(&phil->t_m);
	if ((phil->time_since_last_meal[*i] + (phil->time_before_death / 1000)) \
		* 1000 < get_time() * 1000)
	{
		pthread_mutex_unlock(&phil->t_m);
		pthread_mutex_lock(&phil->alive_check);
		phil->alive = false;
		pthread_mutex_unlock(&phil->alive_check);
	}
	else
		pthread_mutex_unlock(&phil->t_m);
}

int	routine_extend1(t_phil *phil, int id, int *meal)
{
	if (check_death(phil) == -1)
		return (-1);
	pthread_mutex_lock(&phil->print_m);
	if (check_death(phil) == -1)
		return (pthread_mutex_unlock(&phil->print_m), -1);
	my_printf(chrono(phil->time), id, "is sleeping\n");
	pthread_mutex_unlock(&phil->print_m);
	if (check_death(phil) == -1)
		return (-1);
	if (ft_sleep(phil->time_to_sleep / 1000, phil) == -1)
		return (-1);
	if (check_death(phil) == -1)
		return (-1);
	pthread_mutex_lock(&phil->init_sup);
	if (*meal == phil->nbr_of_eat)
		return (pthread_mutex_unlock(&phil->init_sup), -1);
	pthread_mutex_unlock(&phil->init_sup);
	pthread_mutex_lock(&phil->print_m);
	if (check_death(phil) == -1)
		return (pthread_mutex_unlock(&phil->print_m), -1);
	my_printf(chrono(phil->time), id, "is thinking\n");
	pthread_mutex_unlock(&phil->print_m);
	return (0);
}

void	*supervisor(void *data)
{
	int		i;
	t_phil	*phil;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->start);
	pthread_mutex_unlock(&phil->start);
	while (1)
	{
		i = 0;
		while (i < phil->nbr_phil)
		{
			actualise_death(phil, &i);
			pthread_mutex_lock(&phil->alive_check);
			if (phil->alive == false)
			{
				pthread_mutex_unlock(&phil->alive_check);
				pthread_mutex_lock(&phil->print_m);
				my_printf(chrono(phil->time), i, "died\n");
				pthread_mutex_unlock(&phil->print_m);
				return (NULL);
			}
			pthread_mutex_unlock(&phil->alive_check);
		}
		i++;
	}
}
