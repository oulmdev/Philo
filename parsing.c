/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 15:33:25 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/09 23:37:00 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9') || c == '+' || c == ' ');
}

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
