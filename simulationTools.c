/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulationTools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 15:15:49 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/13 15:49:29 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "philo.h"

/*  In my linked list, I have a monitor shared by all the philos.
    this function is used to increment the variable eat_count each time
    this philo eats.
    then i check if that variable is equal to nb_eat
    if it is, i increment the variable nb_eat_max which is used to check if 
    all the philos have eaten nb_eat times.
    then i unlock the mutexes of the forks and the mutex eating and return false
    to stop the thread.

    NOTE: if nb_eat_max is equal to nb_philos, it means that all the philos have
    eaten.
*/
bool	increment_nb_eat_max(t_philo *tmp)
{
	if (((t_monitor *)tmp->monitor)->nb_eat != -1)
			tmp->eat_count++ ;
	pthread_mutex_lock(&((t_monitor *)tmp->monitor)->eating);
	if (tmp->eat_count == ((t_monitor *)tmp->monitor)->nb_eat)
		return (((t_monitor *)tmp->monitor)->nb_eat_max++,
			pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating),
			pthread_mutex_unlock(&tmp->fork),
			pthread_mutex_unlock(&tmp->next->fork),
			false);
	pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating);
	return (true);
}

/*  This function is used to take the forks and eat.
    NOTE: if one of the philos dies, i unlock the mutexes of the forks and
    return false to stop the thread.
*/
bool	take_fork_and_eat(t_philo *tmp)
{
	pthread_mutex_lock(&tmp->fork);
	if (print_it(tmp, "has taken a fork") == false
		|| (tmp->id == 1 && tmp->next->id == 1))
	{
		if (tmp->id == 1 && tmp->next->id == 1)
		{
			ft_usleep(((t_monitor *)tmp->monitor)->time_to_die, tmp);
			print_it(tmp, "died");
		}
		return (pthread_mutex_unlock(&tmp->fork), false);
	}
	pthread_mutex_lock(&tmp->next->fork);
	if (print_it(tmp, "has taken a fork") == false)
		return (pthread_mutex_unlock(&tmp->fork),
			pthread_mutex_unlock(&tmp->next->fork), false);
	if (print_it(tmp, "is eating") == false)
		return (pthread_mutex_unlock(&tmp->fork),
			pthread_mutex_unlock(&tmp->next->fork), false);
	return (true);
}

/*  This function is used to print that a philo died 
    then set the variable is_dead to true to stop the threads.
*/
void	*dead_philo(t_philo *tmp)
{
	return (
		print_it(tmp, "died"),
		pthread_mutex_lock(&((t_monitor *)tmp->monitor)->dead),
		((t_monitor *)tmp->monitor)->is_dead = true,
		pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->dead),
		pthread_mutex_unlock(&(tmp->last_eat_mutex)), NULL);
}
