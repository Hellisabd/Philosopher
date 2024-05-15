/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:05 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/15 14:06:41 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

t_phil	*fill_phil(t_phil *phil, char **argv, int argc)
{
	phil->nbr_phil = ft_atoi(argv[1]);
	phil->time_before_death = ft_atoi(argv[2]);
	phil->time_to_eat = ft_atoi(argv[3]);
	phil->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 5)
		phil->nbr_of_eat = ft_atoi(argv[5]);
	else
		phil->nbr_of_eat = -1;
	if (phil->nbr_phil < 0 || phil->time_before_death < 0 || phil->time_to_eat < 0
		|| phil->time_to_sleep < 0 || (phil->nbr_of_eat != -1 && argc != 6))
		return(NULL);
	return (phil);
}

int	main(int argc, char **argv)
{
	struct timeval tv;
	t_phil	*phil;

	phil = NULL;
	if (argc == 5 || argc == 6)
		phil = fill_phil(phil, argv, argc);
	else
		return (0);
	if (!phil)
		return (printf("Wrong input"), 1);
	printf("phil->nbr_phil: %d\n", phil->nbr_phil);
	printf("phil->time_before_death %d\n", phil->time_before_death);
	printf("phil->time_to_eat: %d\n", phil->time_to_eat);
	printf("phil->time_to_sleep: %d\n", phil->time_to_sleep);
	printf("phil->nbr_of_eat %d\n", phil->nbr_of_eat);
}