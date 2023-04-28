/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud@student.1337.ma <moulmoud>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 22:37:12 by moulmoud@st       #+#    #+#             */
/*   Updated: 2023/04/24 22:37:12 by moulmoud@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(char *str)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
			str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			break ;
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n);
}

int lst_addback(t_lst **philo, t_lst *new)
{
	t_lst	*last;

	if (!new)
		return (1);
	if (!*philo)
	{
		*philo = new;
		new->next = (*philo);
		return (0);
	}
	last = *philo;
	while (last->next != (*philo))
		last = last->next;
	last->next = new;
	new->next = *philo;
	return (0);
}

long long  get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_usleep(long long time)
{
	long long start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
}


t_lst *new_lst(char *av[], int i)
{
	t_lst	*new;

	new = malloc(sizeof(t_lst));
	if (!new)
		return (NULL);
	new->id = i;
	new->n_philo = ft_atoi(av[1]);
	new->t_die = ft_atoi(av[2]);
	new->t_eat = ft_atoi(av[3]);
	new->t_sleep = ft_atoi(av[4]);
	new->n_eat = (av[5]) ? ft_atoi(av[5]) : -1;
	new->t_start = get_time();
	new->t_last_eat = new->t_start;
	new->t_end = new->t_start + new->t_die;
	new->n_eat_count = 0;
	new->is_dead = 0;
	new->is_full = 0;
	new->next = NULL;
	return (new);
}

int check_args(int ac, char *av[])
{
	int	i;
	int	j;

	i = 0;
	if (ac < 5 || ac > 6)
		return (1);
	while (++i < ac)
	{
		j = -1;
		while (av[i][++j])
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
	}
	return (0);
}

int init_philos(t_lst **philo, char *av[])
{
	int	i;

	i = 0;
	while (++i <= ft_atoi(av[1]))
		if (lst_addback(philo, new_lst(av, i)))
			return (1);
	return (0);
}

int init_mutexes(t_lst **philo, int ac)
{
	t_lst	*tmp;
	int		i;

	tmp = *philo;
	i = 0;
	while (i < tmp->n_philo)
	{
		if (pthread_mutex_init(&tmp->fork, NULL))
			return (1);
		if (pthread_mutex_init(&tmp->print, NULL))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int init(t_lst **philo, int ac, char *av[])
{
	if (check_args(ac, av))
		return (1);
	if (init_philos(philo, av))
		return (1);
	if (init_mutexes(philo, ac))
		return (1);
	return (0);
}

void	print_it(t_lst *philo, char *message)
{
	pthread_mutex_lock(&philo->print);
	printf("%lld  Philo %d %s\n",
		get_time() - philo->t_start, philo->id, message);
	pthread_mutex_unlock(&philo->print);
}

int is_full(t_lst *philo)
{
	if (philo->n_eat == -1)
		return (1);	
	if (philo->n_eat_count == philo->n_eat)
	{
		philo->is_full = 1;
		return (0);
	}
	return (1);
}

void *check_dead(void *arg)
{
	t_lst	*philo;

	philo = (t_lst *)arg;
	while (1)
	{
		if (get_time() - philo->t_last_eat > philo->t_die)
		{
			print_it(philo, RED"died"RESET);
			philo->is_dead = 1;
			pthread_mutex_unlock(&philo->dead);
			pthread_mutex_unlock(&philo->dead); 
			return (NULL);
		}
		philo = philo->next;
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_lst	*philo;

	philo = (t_lst *)arg;
	pthread_t dead_thread;
	pthread_create(&philo->dead_thread, NULL, &check_dead, philo);
	pthread_detach(philo->dead_thread);
	while (is_full(philo))
	{
		pthread_mutex_lock(&philo->fork);
		print_it(philo, "has taken a fork");
		pthread_mutex_lock(&philo->next->fork);
		print_it(philo, "has taken a fork");
		print_it(philo, "is eating");
		philo->t_last_eat = get_time();
		ft_usleep(philo->t_eat);
		pthread_mutex_unlock(&philo->next->fork);
		pthread_mutex_unlock(&philo->fork);
		philo->n_eat_count++;
		if (philo->is_dead)
			break ;
		if (philo->n_eat_count == philo->n_eat)
		{
			philo->is_full = 1;
			break ;
		}
		print_it(philo, "is sleeping");
		ft_usleep(philo->t_sleep);
		print_it(philo, "is tinking");
	}
	return (NULL);
}



int philosophers(t_lst *philo)
{
	t_lst	*tmp;
	int		i;

	tmp = philo;
	i = 0;
	pthread_mutex_lock(&philo->dead);
	while (i < tmp->n_philo)
	{
		pthread_create(&tmp->thread, NULL, &routine, tmp);
		tmp = tmp->next;
		usleep(100);
		i++;
	}
	tmp = philo;

	i = 0;

	while (i < tmp->n_philo)
	{
		pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
		i++;
	}
	pthread_mutex_lock(&philo->dead);
	return (0);
}


int destroy_mutexes(t_lst *philo)
{
	t_lst	*tmp;
	int		i;

	tmp = philo;
	i = 0;

	while (i < tmp->n_philo)
	{
		pthread_mutex_destroy(&tmp->fork);
		pthread_mutex_destroy(&tmp->print);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int main(int ac, char *av[])
{
	t_lst	*philo;

	philo = NULL;
	if (ac < 5 || ac > 6)
		return (printf("Error:  number of arguments\n"));
	if (init(&philo, ac, av))
		return (printf("Error:  arguments\n"));	
	if (philosophers(philo))
		return (printf("Error: Failed to create threads\n"));
	if (destroy_mutexes(philo))
		return (printf("Error: Failed to destroy mutexes\n"));
	return (0);
}