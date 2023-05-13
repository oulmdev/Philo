/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 15:33:25 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/13 13:19:47 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9') || c == '+' || c == ' ');
}

/*	Checks if the arguments are valid.
*/
bool	parsing(int ac, char *av[])
{
	int	i;
	int	j;

	i = 0;
	while (++i < ac)
	{
		j = -1;
		if (av[i] == NULL || av[i][0] == '\0')
			return (false);
		while (av[i][++j])
			if (!ft_isdigit(av[i][j]))
				return (false);
	}
	return (true);
}
