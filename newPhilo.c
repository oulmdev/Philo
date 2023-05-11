/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newPhilo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 15:42:49 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/09 16:23:09 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

t_philo	*newphilo(int id, t_monitor **monitor)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->eat_count = 0;
	philo->monitor = (*monitor);
	philo->is_full = false;
	philo->last_eat = philo->monitor->start_time;
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->last_eat_mutex, NULL);
	philo->next = NULL;
	return (philo);
}
