/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moulmoud <moulmoud@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:06:42 by moulmoud          #+#    #+#             */
/*   Updated: 2023/05/03 15:36:08 by moulmoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

int stop_simulation(t_lst *philo, int what);
void set_struct(t_lst *philo);

int ft_atoi(char *str)
{
	int	i;
	int n;
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
	new->n_eat_count = 0;
	new->is_dead = false;
	new->is_full = false;
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
		return (2);
	if (init_mutexes(philo, ac))
		return (init_mutexes(philo, ac));
	return (0);
}

void	print_it(t_lst *philo, char *message)
{
	t_lst	*tmp;

	tmp = philo;
	if ((philo->n_eat != -1 && philo->n_eat_count == philo->n_eat) || philo->final)
		return ;
	while (tmp->id != 1)
		tmp = tmp->next;
	pthread_mutex_lock(&tmp->stop);
	if (philo->is_dead || philo->final)
	{
		pthread_mutex_unlock(&tmp->stop);
		return ;
	}
	printf("%lld  Philo %d %s\n",
		get_time() - philo->t_start, philo->id, message);
	pthread_mutex_unlock(&tmp->stop);
}

int is_full(t_lst *philo)
{
	if (philo->n_eat == -1)
		return (1);

	int i;
	i = 0;
	while (i < philo->n_philo)
	{
		if (philo->n_eat_count < philo->n_eat)
			return (0);
		philo = philo->next;
		i++;
	}
	set_struct(philo);
	return (1);
}

void set_struct(t_lst *philo)
{
	int i;

	i = 0;
	while (i < philo->n_philo)
	{
		philo->is_dead = true;
		philo->final = true;
		philo = philo->next;
		i++;
	}

}

void *check_dead(void *arg)
{
	t_lst	*philo;

	philo = (t_lst *)arg;
	t_lst *tmp;

	tmp = philo;
	int i = 0;
	while (!philo->is_dead && !philo->final)
	{
		i = 0;
		while (i < philo->n_philo)
		{
			if (get_time() - philo->t_last_eat > philo->t_die && !philo->is_dead && !philo->final)
			{
				print_it(tmp, RED"died"RESET);
				set_struct(philo);
				return (NULL);
			}
			philo = philo->next;
			usleep(100);
			i++;
		}
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_lst	*philo;

	philo = (t_lst *)arg;
	pthread_t dead_thread;
	while (!philo->is_full && !philo->is_dead && !philo->final)
	{
		pthread_mutex_lock(&philo->fork);
		print_it(philo, "has taken a fork");
		if (philo->n_philo == 1)
		{
			print_it(philo, RED"died"RESET);
		 	return arg;
		}
		pthread_mutex_lock(&philo->next->fork);
		print_it(philo, "has taken a fork");
		print_it(philo, "is eating");
		philo->t_last_eat = get_time();
		ft_usleep(philo->t_eat);
		pthread_mutex_unlock(&philo->next->fork);
		pthread_mutex_unlock(&philo->fork);
		philo->n_eat_count++;
		if ((philo->n_eat != -1 && philo->n_eat_count == philo->n_eat) || philo->final)
			break ;
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
	pthread_t manager_thread;
	pthread_mutex_init(&tmp->stop, NULL);
	while (i < tmp->n_philo)
	{
		if (pthread_create(&tmp->thread, NULL, &routine, tmp))
			return (1);
		pthread_create(&manager_thread, NULL, &check_dead, tmp);
		pthread_detach(manager_thread);
		tmp = tmp->next;
		i++;
		usleep(100);
	}
	tmp = philo;
	i = 0;
	while (i < tmp->n_philo)
	{
		if (pthread_join(tmp->thread, NULL))
			return (1);
		tmp = tmp->next;
		i++;
	}
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
		return (printf("Error :  %d\n", init(&philo, ac, av)));
	if (philosophers(philo))
		return (printf("Error: Failed to create threads\n"));
	if (destroy_mutexes(philo))
		return (printf("Error: Failed to destroy mutexes\n"));
	return (0);
}
