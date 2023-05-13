/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clearResource.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 21:46:52 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/13 13:49:43 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "philo.h"

/*	Free all the resources used by the program. && destroy all the mutexes.
**	PARAM: philos: the list of philosophers.
*/
void	clearresource(t_philo *philos)
{
	t_philo	*tmp;
	int		num_philo;

	usleep(1000);
	pthread_mutex_destroy(&philos->monitor->dead);
	pthread_mutex_destroy(&philos->monitor->eating);
	pthread_mutex_destroy(&philos->monitor->print);
	num_philo = philos->monitor->nb_philos;
	while (num_philo--)
	{
		tmp = philos;
		philos = philos->next;
		pthread_mutex_destroy(&tmp->fork);
		pthread_mutex_destroy(&tmp->last_eat_mutex);
		free(tmp);
	}
}
