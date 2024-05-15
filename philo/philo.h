/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:45 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/15 13:55:27 by bgrosjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <stdint.h>
# include <stdarg.h>
# include <stdint.h>
# include <limits.h>
# include <stdbool.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include <stdio.h>

typedef struct s_phil
{
	ssize_t		time_to_eat;
	ssize_t		time_to_sleep;
	ssize_t		time_before_death;
	ssize_t		nbr_of_eat;
	ssize_t	timestart;
	ssize_t	timeend;
	int		nbr_phil;
}	t_phil;

//LIBFT

ssize_t	ft_atoi(const char *nptr);

#endif 