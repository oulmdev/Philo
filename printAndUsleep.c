/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printAndUsleep.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:47:58 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/13 14:43:05 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "philo.h"

/*	Print the message if the philo is not dead.
**	if the philo is dead, return false. and unlock the mutexes.
	else print the message and unlock the mutexes.
	then return true.
*/
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

/*	Sleep for time milliseconds.
**	If the philo is dead return, else sleep.

**	This function is used to sleep for a specific time.
**	I used it because usleep is not accurate. because of the scheduler.
	EXAMPLE:
		if we want to sleep for 1000 milliseconds,
		usleep will sleep for 1000 milliseconds 
		+ the time that the scheduler will take to switch to another thread.
		so to prevent this, I used this function.
	->	also it protects from overflow. if the time is too big.
*/
void	ft_usleep(long time, t_philo *philo)
{
	long	start;

	(void)philo;
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
