/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startSimulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:32:31 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/11 21:25:31 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "philo.h"

void	*routine(void *philo)
{
	t_philo	*tmp;

	tmp = (t_philo *)philo;
	while (1)
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
			return (pthread_mutex_unlock(&tmp->fork), NULL);
		}
		pthread_mutex_lock(&tmp->next->fork);
		if (print_it(tmp, "has taken a fork") == false)
			return (pthread_mutex_unlock(&tmp->fork),
				pthread_mutex_unlock(&tmp->next->fork), NULL);
		if (print_it(tmp, "is eating") == false)
			return (pthread_mutex_unlock(&tmp->fork),
				pthread_mutex_unlock(&tmp->next->fork), NULL);
		pthread_mutex_lock(&(tmp->last_eat_mutex));
		tmp->last_eat = get_time();
		pthread_mutex_unlock(&(tmp->last_eat_mutex));
		if (((t_monitor *)tmp->monitor)->nb_eat != -1)
			tmp->eat_count++ ;
		pthread_mutex_lock(&((t_monitor *)tmp->monitor)->eating);//
		if (tmp->eat_count == ((t_monitor *)tmp->monitor)->nb_eat)
		{
			((t_monitor *)tmp->monitor)->nb_eat_max++;
			tmp->is_full = true;
			pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating);
			pthread_mutex_unlock(&tmp->fork);
			pthread_mutex_unlock(&tmp->next->fork);
			return (NULL);
		}
		pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating);//
		ft_usleep(((t_monitor *)tmp->monitor)->time_to_eat, tmp);
		pthread_mutex_unlock(&tmp->fork);
		pthread_mutex_unlock(&tmp->next->fork);
		if (tmp->eat_count == ((t_monitor *)tmp->monitor)->nb_eat)
			return (tmp->is_full = true, NULL);
		if (print_it(tmp, "is sleeping") == false)
			break ;
		ft_usleep(((t_monitor *)tmp->monitor)->time_to_sleep, tmp);
		if (print_it(tmp, "is thinking") == false)
			break ;
	}
	return (NULL);
}

# define RED "\033[0;31m"
# define RESET "\033[0m"

void	*monitor(void *philo)
{
	t_philo	*tmp;

	tmp = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&((t_monitor *)tmp->monitor)->dead);
		pthread_mutex_lock(&((t_monitor *)tmp->monitor)->eating);//
		if (((t_monitor *)tmp->monitor)->is_dead == true || ((t_monitor *)tmp->monitor)->nb_eat_max == ((t_monitor *)tmp->monitor)->nb_philos)
			return (pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->dead), pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating), NULL);
		pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->eating);//
		pthread_mutex_lock(&(tmp->last_eat_mutex));
		if (get_time() - tmp->last_eat
			> ((t_monitor *)tmp->monitor)->time_to_die
			&& (t_monitor *)tmp->monitor->is_dead == false)
			return (pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->dead),
				print_it(tmp, RED"died"RESET),
				pthread_mutex_lock(&((t_monitor *)tmp->monitor)->dead),
				((t_monitor *)tmp->monitor)->is_dead = true,
				pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->dead),
				pthread_mutex_unlock(&(tmp->last_eat_mutex)), NULL);
		pthread_mutex_unlock(&((t_monitor *)tmp->monitor)->dead);
		pthread_mutex_unlock(&(tmp->last_eat_mutex));
		tmp = tmp->next;
		usleep(100);
	}
	return (NULL);
}

bool	join_threads(t_philo *philos)
{
	int	nb_philos;

	nb_philos = ((t_monitor *)philos->monitor)->nb_philos;
	while (nb_philos--)
	{
		if (pthread_join(philos->thread, NULL))
			return (false);
		philos = philos->next;
	}
	return (true);
}

bool	start_simulation(t_philo *philos)
{
	t_philo		*tmp;
	int			nb_philos;
	pthread_t	monitor_thread;

	tmp = philos;
	nb_philos = ((t_monitor *)tmp->monitor)->nb_philos;
		if (!(tmp->id == 1 && tmp->next->id == 1))
		{
			if (pthread_create(&monitor_thread, NULL, monitor, tmp))
				return (false);
			pthread_detach(monitor_thread);
		}
	while (nb_philos--)
	{
		if (pthread_create(&tmp->thread, NULL, routine, tmp))
			return (false);
		usleep(100);
		tmp = tmp->next;
	}
	return (join_threads(philos));
}
