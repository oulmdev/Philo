/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printAndUsleep.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:47:58 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/11 16:23:17 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "philo.h"

bool	print_it(t_philo *philo, char *str)
{
	pthread_mutex_lock(&((t_monitor *)philo->monitor)->dead);
	pthread_mutex_lock(&((t_monitor *)philo->monitor)->print);
	if (((t_monitor *)philo->monitor)->is_dead == true)
	{
		pthread_mutex_unlock(&((t_monitor *)philo->monitor)->dead);
		pthread_mutex_unlock(&((t_monitor *)philo->monitor)->print);
		return (false);
	}
	else
	{
		printf("%lld %d %s\n", get_time()
			- ((t_monitor *)philo->monitor)->start_time, philo->id, str);
		pthread_mutex_unlock(&((t_monitor *)philo->monitor)->print);
		pthread_mutex_unlock(&((t_monitor *)philo->monitor)->dead);
	}
	return (true);
}

void	ft_usleep(long time, t_philo *philo)
{
	long	start;

	start = get_time();
	while (get_time() - start < time)
	{
		usleep(100);
		pthread_mutex_lock(&((t_monitor *)philo->monitor)->dead);
		if (((t_monitor *)philo->monitor)->is_dead == true)
		{
			pthread_mutex_unlock(&((t_monitor *)philo->monitor)->dead);
			return ;
		}
		pthread_mutex_unlock(&((t_monitor *)philo->monitor)->dead);
	}
}
