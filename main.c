/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 12:47:06 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/07/13 12:08:03 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	init_data(t_data *data)
{
	int	i;

	data->someone_died = false;
	data->start_time_ms = timestamp_ms();
	pthread_mutex_init(&data->print_lock, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
}

static void	spawn_philosophers(t_data *data, t_philo *philos,
		pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i;
		philos[i].last_meal_ms = data->start_time_ms;
		philos[i].meals_taken = 0;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].rules = data;
		pthread_create(&threads[i], NULL, philosopher_routine, &philos[i]);
		i++;
	}
}

static void	cleanup(t_data *data, t_philo *philos, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->print_lock);
	free(data->forks);
	free(philos);
	free(threads);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	if (!parse_args(argc, argv, &data))
	{
		write(2, "Usage: ./philo nbr time_die time_eat time_sleep [max]\n", 56);
		return (1);
	}
	init_data(&data);
	philos = malloc(sizeof(t_philo) * data.num_philos);
	threads = malloc(sizeof(pthread_t) * data.num_philos);
	spawn_philosophers(&data, philos, threads);
	pthread_create(&monitor, NULL, monitor_routine, philos);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data.num_philos)
		pthread_detach(threads[i++]);
	cleanup(&data, philos, threads);
	return (0);
}
