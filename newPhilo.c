/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newPhilo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 15:42:49 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/13 15:29:06 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*	Getting the current time in milliseconds.
	The struct timeval is used to hold the current time of day in seconds
	and microseconds. 
	The tv_sec field holds the number of seconds since
	the Unix epoch (midnight on January 1, 1970).
	tv_usec field holds the number of microseconds since the last full second.

	so to calculate the time in miliseconds we multiply the seconds by 1000
	and add the microseconds divided by 1000.
	then we return the sum of the two.
	EXAMPLE:
		seconds = 1620925288
		microseconds = 123456
		(1620925200 * 1000) + (123456 / 1000) = 1620925288123
*/
long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long long	ft_atoi(char *str)
{
	int	i;
	int	nbr;

	i = 0;
	nbr = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i] && str[i] == '+')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
	}
	return (nbr);
}

/*	create new philo ,and init mutexes.
*/
t_philo	*newphilo(int id, t_monitor **monitor)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->eat_count = 0;
	philo->monitor = (*monitor);
	philo->last_eat = philo->monitor->start_time;
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->last_eat_mutex, NULL);
	philo->next = NULL;
	return (philo);
}
