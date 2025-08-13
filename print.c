/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 12:39:04 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/08/13 18:42:16 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"

void	print_log(t_data *rules, int philo_id, const char *msg)
{
	long long	time;
	int			philo_num;

	time = timestamp_ms() - rules->start_time_ms;
	philo_num = philo_id;
	pthread_mutex_lock(&rules->print_lock);
	if (!rules->someone_died || msg[0] == 'd')
	{
		printf("%lld %d %s", time, philo_num, msg);
		if (msg[0] == 'd')
			rules->someone_died = 1;
	}
	pthread_mutex_unlock(&rules->print_lock);
}
