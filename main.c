#include "philo.h"

int	main(int ac, char **av)
{
	t_philo			input;
	t_philo			*philos;
	pthread_mutex_t	printing_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meals_lock;

	if (ac != 5 && ac != 6)
		exit(0);
	parse_input(&input, av);
	pthread_mutex_init(&printing_lock, NULL);
	pthread_mutex_init(&death_lock, NULL);
	pthread_mutex_init(&meals_lock, NULL);
	philos = malloc(input.nbr_philos * sizeof(t_philo));
	if (!philos)
		exit(1);
	ft_initialize(philos, input, &printing_lock, &death_lock);
	create_threads(philos, simulate_dinner);
	while (1)
	{
		if (!everyone_alive(philos) || !philos_still_hungry(philos))
			break ;
//		printf("%d\n", everyone_alive(philos));
	}
//	printf("All philosophers are full\n");
	pthread_mutex_destroy(&printing_lock);
	pthread_mutex_destroy(&death_lock);
	ft_clear(philos);
}
