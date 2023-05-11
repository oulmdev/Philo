/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addBack.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 15:38:30 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/08 13:53:13 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	lst_addback(t_philo **philo, t_philo *new)
{
	t_philo	*last;

	if (!new)
		return (true);
	if (!*philo)
	{
		*philo = new;
		new->next = (*philo);
		return (false);
	}
	last = *philo;
	while (last->next != (*philo))
		last = last->next;
	last->next = new;
	new->next = *philo;
	return (false);
}
