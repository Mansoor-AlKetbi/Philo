/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:25:09 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/06/15 18:37:29 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_philo
{
	int	id;
	long long last_meal_ms;
	int meals_taken;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_data			*rules;
}t_philo;


typedef struct s_data
{
	int			num_philos; //Total philosophers (N)
	long long	time_to_die; //ms before starving
	long long	time_to_eat; //ms spent eating
	long long	time_to_sleep; //ms spent sleeping
	int			max_meals; // or -1 if unlimited
	pthread_mutex_t *forks; // array of N mutexes
	pthread_mutex_t print_lock; // serialize console input
	long long start_time_ms; // timestamp when you launched
	bool someone_died; // flag watched by  monitor
}t_data;

size_t ft_strlen(char *str);
int ft_atoi(char *str);
int is_digit(char *arg);
long long timestamp_ms(void);
void print_log(t_data *rules, int philo_id, const char *curr);
static void take_forks(t_philo *ph);
static void put_forks(t_philo *ph);
void *philosopher_routine(void *arg);
void    *monitor_routine(void *arg);
#endif