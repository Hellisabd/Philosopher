/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:55 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/06/04 15:16:27 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	my_printf(ssize_t time, int id, char *str)
{
	char	*clr1;

	if (id % 5 == 0)
		clr1 = PURPLE;
	if (id % 5 == 1)
		clr1 = ORANGE;
	if (id % 5 == 2)
		clr1 = GREEN;
	if (id % 5 == 3)
		clr1 = BLUE;
	if (id % 5 == 4)
		clr1 = RED;
	printf("%s%ld %d %s%s", clr1, time, id, str, NC);
}

int	ft_sleep(ssize_t end, t_phil *phil)
{
	ssize_t	time;

	time = get_time();
	while (get_time() < time + end)
	{
		if (check_death(phil) == -1)
			return (-1);
	}
	return (0);
}

ssize_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / 1000);
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
