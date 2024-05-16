/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:05 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/16 13:26:43 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

t_phil	fill_phil(char **argv, int argc)
{
	t_phil	phil;
	
	phil.nbr_phil = ft_atoi(argv[1]);
	phil.time_before_death = ft_atoi(argv[2]);
	phil.time_to_eat = ft_atoi(argv[3]);
	phil.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		phil.nbr_of_eat = ft_atoi(argv[5]);
	else
		phil.nbr_of_eat = -1;
	return (phil);
}

t_phil	init_phil(int argc, char **argv)
{
	t_phil	phil;
	
	phil.err = 0;
	phil.i = 1;
	while (argv[phil.i])
			if (ft_strisdigit(argv[phil.i++]) == 0)
				phil.err = -1;
	if (argc == 5 || argc == 6)
		phil = fill_phil(argv, argc);
	else
		phil.err = -1;
	if (phil.nbr_phil < 0 || phil.time_before_death < 0 || phil.time_to_eat < 0
		|| phil.time_to_sleep < 0 || (phil.nbr_of_eat != -1 && argc != 6))
		phil.err = -1;
	phil.i = 0;
	pthread_mutex_init(&phil.mutex, NULL);
	phil.counter = 0;
	phil.id = malloc(sizeof(pthread_t) * (phil.nbr_phil));
	return (phil);
}

int	main(int argc, char **argv)
{
	t_phil	phil;
	
	phil = init_phil(argc, argv);
	// if ()
	while (phil.i < phil.nbr_phil)
	{
		pthread_create(&phil.id[phil.i], NULL, routine, &phil);
		pthread_mutex_lock(&phil.mutex);
		phil.i++;
		pthread_mutex_unlock(&phil.mutex);
	}
	phil.i = 0;
	while (phil.i < phil.nbr_phil)
	{
		pthread_join(phil.id[phil.i], NULL);
		phil.i++;
	}
	pthread_mutex_destroy(&phil.mutex);
	free(phil.id);
	return (0);
}
