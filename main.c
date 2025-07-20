/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 12:47:06 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/07/20 17:19:38 by mal-ketb         ###   ########.fr       */
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
	if (!data->forks)
		exit(1);
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
}

static void	launch_philos(t_data *data, t_philo *philos, pthread_t *threads,
		int i)
{
	philos[i].id = i + 1;
	philos[i].last_meal_ms = data->start_time_ms;
	philos[i].meals_taken = 0;
	philos[i].left_fork = &data->forks[i];
	philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
	philos[i].rules = data;
	pthread_create(&threads[i], NULL, philosopher_routine, &philos[i]);
}

static void	spawn_philosophers(t_data *data, t_philo *philos,
		pthread_t *threads)
{
	int	i;

	if (data->num_philos == 1)
	{
		launch_philos(data, philos, threads, 0);
		return ;
	}
	i = 0;
	while (i < data->num_philos)
	{
		launch_philos(data, philos, threads, i);
		i += 2;
	}
	usleep(data->time_to_eat * 500);
	i = 1;
	while (i < data->num_philos)
	{
		launch_philos(data, philos, threads, i);
		i += 2;
	}
}

static void	cleanup(t_data *data, t_philo *philos, pthread_t *threads)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_lock);
	if (philos)
		free(philos);
	if (threads)
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
		write(2, "Error: Invalid arguments\n", 25);
		return (1);
	}
	init_data(&data);
	philos = malloc(sizeof(t_philo) * data.num_philos);
	threads = malloc(sizeof(pthread_t) * data.num_philos);
	if (!philos || !threads)
		return (1);
	spawn_philosophers(&data, philos, threads);
	pthread_create(&monitor, NULL, monitor_routine, philos);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < data.num_philos)
		pthread_join(threads[i], NULL);
	cleanup(&data, philos, threads);
	return (0);
}
