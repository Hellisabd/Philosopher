/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:55 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/24 10:55:43 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

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
