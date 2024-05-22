/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:55 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/22 16:53:22 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

ssize_t	get_current_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec);
}
