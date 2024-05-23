/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:55 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/23 15:55:36 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	my_printf(char *clr1, ssize_t time, int id, char *str)
{
	printf("%s%ld %d %s%s", clr1, time, id, str, NC);
}

int	ft_sleep(ssize_t end, t_phil *phil, int id)
{
	ssize_t	time;

	time = get_time();
	while (get_time() < time + end)
	{
		if (supervisor(phil, id) == -1)
			return (-1);
	}
	return (0);
}

ssize_t	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / 1000);
}
