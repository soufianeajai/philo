#include "philo.h"

int	main(int ac, char **av)
{
	t_philo			input;
	t_data			data;
	pthread_t		waiter;

	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) <= 0)
		exit(0);
	parse_input(&input, av);
	data.philos = malloc(input.nbr_philos * sizeof(t_philo));
	if (!data.philos)
		exit(1);
	ft_initialize(&data, input);
	create_threads(data.philos, simulate_dinner, &waiter);
	ft_clear(&data);
//	system("leaks philo");
	return (0);
 }
