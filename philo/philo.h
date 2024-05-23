/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrosjea <bgrosjea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:56:45 by bgrosjea          #+#    #+#             */
/*   Updated: 2024/05/23 15:55:30 by bgrosjea         ###   ########.fr       */
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
# include <stdio.h>

# define NC "\e[0m"
# define RED "\e[1;31m"
# define GREEN "\e[1;32m"
# define YELLOW "\e[1;33m"
# define ORANGE "\e[1;34m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;36m"

typedef struct s_pers
{
	int	name;
}	t_pers;


typedef struct s_phil
{
	ssize_t				time_to_eat;
	ssize_t				time_to_sleep;
	ssize_t				time_before_death;
	ssize_t				nbr_of_eat;
	ssize_t				time;
	ssize_t				nbr_phil;
	ssize_t				*time_since_last_meal;
	pthread_t			*id;
	int					counter;
	bool				alive;
	int					i;
	pthread_mutex_t		mutex;
	pthread_mutex_t		init_sup;
	pthread_mutex_t		print_m;
	pthread_mutex_t    start;
	pthread_mutex_t    *f_m;
	pthread_mutex_t    t_m;
	pthread_mutex_t    alive_check;
	int					err;
	bool					*is_eating;
	int					starting;
}	t_phil;

//LIBFT

int	ft_strisdigit(char *s);
ssize_t	ft_atoi(const char *nptr);

//THREAD

int	supervisor(t_phil *phil, int id);
void	*routine(void *data);

//UTILS

void	my_printf(char *clr1, ssize_t time, int id, char *str);
int	ft_sleep(ssize_t end, t_phil *phil, int id);
ssize_t	get_time(void);

#endif 