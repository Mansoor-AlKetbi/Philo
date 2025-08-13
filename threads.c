/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:06:40 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/08/13 18:44:11 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *ph)
{
	if (ph->rules->num_philos == 1)
	{
		pthread_mutex_lock(ph->left_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
		precise_sleep_routine(ph->rules->time_to_die, ph->rules);
		pthread_mutex_unlock(ph->left_fork);
		return ;
	}
	if (ph->id % 2 == 0)
	{
		pthread_mutex_lock(ph->right_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
		pthread_mutex_lock(ph->left_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(ph->left_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
		pthread_mutex_lock(ph->right_fork);
		print_log(ph->rules, ph->id, "has taken a fork\n");
	}
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
	t_data		*d;
	int			i;
	int			finished;
	long long	now;

	d = phs->rules;
	i = 0;
	finished = 0;
	now = timestamp_ms();
	while (i < d->num_philos)
	{
		pthread_mutex_lock(&phs[i].meal_lock);
		if (now - phs[i].last_meal_ms >= d->time_to_die)
		{
			pthread_mutex_unlock(&phs[i].meal_lock);
			return (i);
		}
		if (d->max_meals > 0 && phs[i].meals_taken >= d->max_meals)
			finished++;
		pthread_mutex_unlock(&phs[i].meal_lock);
		i++;
	}
	if (d->max_meals > 0 && finished == d->num_philos)
		return (-2);
	return (-1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		usleep(1000);
	while (!stop_flag(ph->rules))
	{
		if (ph->rules->num_philos % 2 == 1)
        	precise_sleep_routine(ph->rules->time_to_eat / 2, ph->rules);
		take_forks(ph);
		if (stop_flag(ph->rules))
		{
			put_forks(ph);
			break ;
		}
		print_log(ph->rules, ph->id, "is eating\n");
		pthread_mutex_lock(&ph->meal_lock);
		ph->last_meal_ms = timestamp_ms();
		pthread_mutex_unlock(&ph->meal_lock);
		precise_sleep_routine(ph->rules->time_to_eat, ph->rules);
		put_forks(ph);
		pthread_mutex_lock(&ph->meal_lock);
		ph->meals_taken++;
		if (ph->rules->max_meals > 0 && ph->meals_taken >= ph->rules->max_meals)
		{
			pthread_mutex_unlock(&ph->meal_lock);
			break ;
		}
		pthread_mutex_unlock(&ph->meal_lock);
		print_log(ph->rules, ph->id, "is sleeping\n");
		precise_sleep_routine(ph->rules->time_to_sleep, ph->rules);
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
