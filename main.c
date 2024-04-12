#include "philo.h"

int	main(int ac, char **av)
{
	t_philo		input;
	t_data		data;
	pthread_t	waiter;

	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) <= 0)
	{
		write(2, "Enter a valid input\n", 21);
		exit(1);
	}
	parse_input(&input, av);
	data.philos = malloc(input.nbr_philos * sizeof(t_philo));
	if (!data.philos)
		return (1);
	ft_initialize(&data, input);
	create_threads(data.philos, simulate_dinner, &waiter);
	ft_clear(&data);
	return (0);
}
