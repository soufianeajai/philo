/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sajaite <sajaite@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:37:33 by sajaite           #+#    #+#             */
/*   Updated: 2024/04/27 19:37:34 by sajaite          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo		input;
	t_data		data;
	pthread_t	waiter;
	int			alloc_err;

	alloc_err = -1;
	if (handle_input_errors(ac, av, &input))
		return (1);
	data.philos = malloc(input.nbr_philos * sizeof(t_philo));
	if (!data.philos)
		return (1);
	init_mutexes(&data);
	ft_initialize(&data, input, &alloc_err);
	if (alloc_err != -1)
		return (ft_clear(&data, alloc_err));
	forks_init(data.philos);
	if (create_threads(data.philos, simulate_dinner, &waiter))
		return (ft_clear(&data, alloc_err));
	return (!ft_clear(&data, alloc_err));
}
