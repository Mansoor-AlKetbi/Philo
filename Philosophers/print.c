/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-ketb <mal-ketb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 12:39:04 by mal-ketb          #+#    #+#             */
/*   Updated: 2025/06/01 12:55:45 by mal-ketb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_log(t_data *rules, int philo_id, const char *curr)
{
	long long t;
	
	t = timestamp_ms() - rules->start_time_ms;
	pthread_mutex_lock(&rules->print_lock);
	if (rules->someone_died == 1)
		printf("%lld %d %s", t, philo_id + 1, curr);
	pthread_mutex_unlock(&rules->print_lock);
}
