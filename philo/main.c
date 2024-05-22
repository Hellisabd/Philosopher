/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:05 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/23 00:18:56 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	fill_phil(char **argv, int argc, t_phil	*phil)
{
	struct timeval tv;
	
	phil->nbr_phil = ft_atoi(argv[1]);
	phil->time_before_death =  1000 * ft_atoi(argv[2]);
	phil->time_to_eat = 1000 * ft_atoi(argv[3]);
	phil->time_to_sleep = 1000 * ft_atoi(argv[4]);
	if (argc == 6)
		phil->nbr_of_eat = ft_atoi(argv[5]);
	else
		phil->nbr_of_eat = -1;
	gettimeofday(&tv, NULL);
	phil->time = tv.tv_usec;
}

void	init_mutex(t_phil *phil)
{
	int	i;

	i = 0;
	pthread_mutex_init(&phil->start, NULL);
	pthread_mutex_init(&phil->t_m, NULL);
	pthread_mutex_init(&phil->alive_check, NULL);
	pthread_mutex_init(&phil->print_m, NULL);
	phil->f_m = malloc(sizeof(pthread_mutex_t) * phil->nbr_phil);
	while (i < phil->nbr_phil)
		pthread_mutex_init(&phil->f_m[i++], NULL);
	pthread_mutex_init(&phil->mutex, NULL);
}

void	init_phil(int argc, char **argv, t_phil *phil)
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
	if (phil->nbr_phil < 0 || phil->time_before_death < 0 || phil->time_to_eat < 0
		|| phil->time_to_sleep < 0 || (phil->nbr_of_eat != -1 && argc != 6))
		phil->err = -1;
	phil->i = -1;
	phil->counter = 0;
	phil->id = malloc(sizeof(pthread_t) * (phil->nbr_phil));
	phil->is_eating = malloc(sizeof(bool) * (phil->nbr_phil));
	phil->time_since_last_meal = malloc(sizeof(ssize_t) * (phil->nbr_phil));
	i = -1;
	while (++i < phil->nbr_phil)
	{
		phil->time_since_last_meal[i] = get_current_time();
		phil->is_eating[i] = false;
	}
	init_mutex(phil);
}

int	main(void)
{
	int argc = 5;
	char *argv[6] = {"prout", "4", "410", "200", "200", NULL};
	t_phil	phil;

	init_phil(argc, argv, &phil);
	phil.alive = true;
	pthread_mutex_lock(&phil.start);
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
	{
		pthread_create(&phil.id[phil.i], NULL, routine, &phil);
		phil.i++;
	}
	pthread_mutex_unlock(&phil.start);
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
	{
		pthread_join(phil.id[phil.i], NULL);
		phil.i++;
	}
	pthread_mutex_destroy(&phil.mutex);
	pthread_mutex_destroy(&phil.alive_check);
	pthread_mutex_destroy(&phil.t_m);
	pthread_mutex_destroy(&phil.start);
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
		pthread_mutex_destroy(&phil.f_m[phil.i++]);
	free(phil.id);
	free(phil.f_m);
	free(phil.time_since_last_meal);
	free(phil.is_eating);
	return (0);
}
