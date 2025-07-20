/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:06:40 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/07/20 17:13:29 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *ph)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (ph->rules->num_philos == 1)
	{
		pthread_mutex_lock(ph->left_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
		while (!ph->rules->someone_died)
			usleep(1000);
		return ;
	}
	if (ph->left_fork < ph->right_fork)
	{
		first = ph->left_fork;
		second = ph->right_fork;
	}
	else
	{
		first = ph->right_fork;
		second = ph->left_fork;
	}
	pthread_mutex_lock(first);
	print_log(ph->rules, ph->id, "has taken a fork\n");
	pthread_mutex_lock(second);
	print_log(ph->rules, ph->id, "has taken a fork\n");
}

static void	put_forks(t_philo *ph)
{
	if (ph->rules->num_philos == 1)
	{
		pthread_mutex_unlock(ph->left_fork);
		return ;
	}
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

static int	check_end(t_philo *phs)
{
	t_data		*data;
	int			i;
	long long	current_time;
	int			meals_finished;

	data = phs->rules;
	i = 0;
	meals_finished = 0;
	current_time = timestamp_ms();
	while (i < phs[0].rules->num_philos)
	{
		pthread_mutex_lock(&data->print_lock);
		if (current_time - phs[i].last_meal_ms >= phs[i].rules->time_to_die)
		{
			pthread_mutex_unlock(&data->print_lock);
			return (i);
		}
		if (data->max_meals > 0 && phs[i].meals_taken >= data->max_meals)
			meals_finished++;
		pthread_mutex_unlock(&data->print_lock);
		i++;
	}
	if (data->max_meals > 0 && meals_finished == data->num_philos)
		return (-2);
	return (-1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		usleep(ph->rules->time_to_eat * 500);
	while (!ph->rules->someone_died)
	{
		take_forks(ph);
		print_log(ph->rules, ph->id, "is eating\n");
		pthread_mutex_lock(&ph->rules->print_lock);
		ph->last_meal_ms = timestamp_ms();
		pthread_mutex_unlock(&ph->rules->print_lock);
		usleep(ph->rules->time_to_eat * 1000);
		put_forks(ph);
		ph->meals_taken++;
		if (ph->rules->max_meals > 0 && ph->meals_taken >= ph->rules->max_meals)
			break ;
		print_log(ph->rules, ph->id, "is sleeping\n");
		usleep(ph->rules->time_to_sleep * 1000);
		print_log(ph->rules, ph->id, "is thinking\n");
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_philo	*phs;
	int		idx;

	phs = (t_philo *)arg;
	while (1)
	{
		idx = check_end(phs);
		if (idx >= 0)
		{
			phs[idx].rules->someone_died = true;
			print_log(phs[idx].rules, phs[idx].id, "died\n");
			break ;
		}
		if (idx == -2)
			break ;
		usleep(100);
	}
	return (NULL);
}
