/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 00:34:20 by moulmoud          #+#    #+#             */
/*   Updated: 2023/04/03 00:34:20 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H

# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>



#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef struct s_lst
{
	int				id; // philo id : done
	int				n_philo; // number of philos : done
	long long		t_die; // time to die : done
	long long		t_eat; // time to eat : done
	long long		t_sleep; // time to sleep : done
	int				n_eat; // number of times each philo must eat : done
	long long		t_start; // time when the simulation started : done
	long long		t_last_eat; // time when the last meal was taken
	int				n_eat_count; // number of times the philo has eaten
	int				is_dead; // 1 if the philo is dead : done
	int				is_full; // 1 if the philo is full : done
	pthread_t		thread; // thread of the philo
	pthread_t		dead_thread; // thread of the death
	pthread_mutex_t	fork; // fork of the philo : done
	pthread_mutex_t	print; // mutex for printing : done
	pthread_mutex_t	stop; // mutex for printing : done
	struct s_lst	*next; // next philo
}	t_lst;

#endif
