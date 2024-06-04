/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:28:46 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/06/04 10:51:46 by bgrosjea         ###   ########.fr       */
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

void	*routine(void *data)
{
	t_phil	*phil;
	int		id;
	int		meal;

	phil = (t_phil *)data;
	meal = 0;
	id = 0;
	ft_start(phil, &id);
	while (1)
	{
		if (one_phil(phil, id) == -1)
			return (NULL);
		pthread_mutex_lock(&phil->alive_check);
		if (phil->alive == false)
			return (pthread_mutex_unlock(&phil->alive_check), NULL);
		pthread_mutex_unlock(&phil->alive_check);
		if (check_death(phil) == -1)
			return (NULL);
		take_fork(phil, id);
		meal++;
		if (routine_extend1(phil, id, &meal) == -1)
			return (NULL);
	}
	return (NULL);
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

void	init_mutex(t_phil *phil)
{
	int	i;

	i = 0;
	pthread_mutex_init(&phil->start, NULL);
	pthread_mutex_init(&phil->init_sup, NULL);
	pthread_mutex_init(&phil->t_m, NULL);
	pthread_mutex_init(&phil->alive_check, NULL);
	pthread_mutex_init(&phil->print_m, NULL);
	phil->f_m = malloc(sizeof(pthread_mutex_t) * phil->nbr_phil);
	while (i < phil->nbr_phil)
		pthread_mutex_init(&phil->f_m[i++], NULL);
	pthread_mutex_init(&phil->mutex, NULL);
}
