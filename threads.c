/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:06:40 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/07/13 12:42:22 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *ph)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

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
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

static int	check_end(t_philo *phs)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	while (i < phs[0].rules->num_philos)
	{
		if (timestamp_ms() - phs[i].last_meal_ms > phs[i].rules->time_to_die)
			return (i);
		if (phs[i].rules->max_meals > 0
			&& phs[i].meals_taken < phs[i].rules->max_meals)
			done = 0;
		i++;
	}
	if (done)
		return (-1);
	return (-2);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (!ph->rules->someone_died)
	{
		take_forks(ph);
		ph->last_meal_ms = timestamp_ms();
		print_log(ph->rules, ph->id, "is eating\n");
		usleep(ph->rules->time_to_eat * 1000);
		ph->meals_taken++;
		put_forks(ph);
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
			print_log(phs[idx].rules,
				phs[idx].id, "died\n");
			break ;
		}
		if (phs[0].rules->max_meals > 0 && idx == -1)
			break ;
		usleep(1000);
	}
	return (NULL);
}
