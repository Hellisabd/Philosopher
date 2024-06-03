/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:05 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/06/03 10:11:19 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	fill_phil(char **argv, int argc, t_phil	*phil)
{	
	phil->nbr_phil = ft_atoi(argv[1]);
	if (phil->nbr_phil > 200)
		phil->nbr_phil = -1;
	phil->time_before_death = 1000 * ft_atoi(argv[2]);
	phil->time_to_eat = 1000 * ft_atoi(argv[3]);
	phil->time_to_sleep = 1000 * ft_atoi(argv[4]);
	if (argc == 6)
		phil->nbr_of_eat = ft_atoi(argv[5]);
	else
		phil->nbr_of_eat = -1;
}

int	init_phil2(t_phil *phil)
{
	int	i;

	phil->i = -1;
	phil->counter = 0;
	phil->id = malloc(sizeof(pthread_t) * (phil->nbr_phil));
	phil->is_eating = malloc(sizeof(bool) * (phil->nbr_phil));
	phil->time_since_last_meal = malloc(sizeof(ssize_t) * (phil->nbr_phil));
	if (!phil->id || !phil->is_eating || !phil->time_since_last_meal)
		return (free(phil->id), free(phil->is_eating), \
		free(phil->time_since_last_meal), -1);
	i = -1;
	while (++i < phil->nbr_phil)
	{
		phil->time_since_last_meal[i] = get_time();
		phil->is_eating[i] = false;
	}
	return (0);
}

int	init_phil(int argc, char **argv, t_phil *phil)
{
	int		i;

	phil->err = -1;
	phil->i = 1;
	phil->alive = true;
	i = 0;
	while (argv[phil->i])
		if (ft_strisdigit(argv[phil->i++]) == 0)
			phil->err = -1;
	if (argc == 5 || argc == 6)
		fill_phil(argv, argc, phil);
	else
		phil->err = -1;
	if (phil->nbr_phil < 0 || phil->time_before_death < 0
		|| phil->time_to_eat < 0
		|| phil->time_to_sleep < 0 || (phil->nbr_of_eat != -1 && argc != 6))
		return (printf("%s%s%s\n"\
		, RED, "Huh? Did u read the fucking manual?", NC), -1);
	if (init_phil2(phil) == -1)
		return (printf("%s%s%s", RED, "Malloc Error", NC), -2);
	init_mutex(phil);
	return (0);
}

void	destroyer(t_phil *phil)
{
	pthread_mutex_destroy(&phil->mutex);
	pthread_mutex_destroy(&phil->alive_check);
	pthread_mutex_destroy(&phil->t_m);
	pthread_mutex_destroy(&phil->start);
	pthread_mutex_destroy(&phil->print_m);
	pthread_mutex_destroy(&phil->init_sup);
	phil->i = 0;
	while (phil->i < phil->nbr_phil)
		pthread_mutex_destroy(&phil->f_m[phil->i++]);
	free(phil->id);
	free(phil->f_m);
	free(phil->time_since_last_meal);
	free(phil->is_eating);
}

int	main(int argc, char **argv)
{
	struct timeval	tv;
	t_phil			phil;

	if (init_phil(argc, argv, &phil) < 0)
		return (1);
	phil.alive = true;
	pthread_mutex_lock(&phil.start);
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
	{
		pthread_create(&phil.id[phil.i], NULL, routine, &phil);
		phil.i++;
	}
	gettimeofday(&tv, NULL);
	phil.time = (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / 1000);
	pthread_mutex_unlock(&phil.start);
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
	{
		pthread_join(phil.id[phil.i], NULL);
		phil.i++;
	}
	destroyer(&phil);
	return (0);
}
