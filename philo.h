/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:25:09 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/07/06 20:09:07 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	long long			last_meal_ms;
	int					meals_taken;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_data				*rules;
}						t_philo;

typedef struct s_data
{
	int					num_philos;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					max_meals;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_lock;
	long long			start_time_ms;
	bool				someone_died;
}						t_data;

int						ft_atoi(const char *str);
int						is_digit(const char *str);
long long				timestamp_ms(void);

void					print_log(t_data *rules, int philo_id, const char *msg);

int						parse_args(int argc, char **argv, t_data *data);

void					*philosopher_routine(void *arg);
void					*monitor_routine(void *arg);

#endif
