#include "philo.h"

void	ft_initialize(t_philo *philos, t_philo input,
		pthread_mutex_t *printing_lock, pthread_mutex_t *death_lock)
{
	int	i;

	i = 0;
	while (i < input.nbr_philos)
	{
		philos[i].id = i + 1;
		philos[i].nbr_philos = input.nbr_philos;
		philos[i].time_to_die = input.time_to_die;
		philos[i].time_to_eat = input.time_to_eat;
		philos[i].time_to_sleep = input.time_to_sleep;
		philos[i].nbr_meals = input.nbr_meals;
		philos[i].start_time = time_now();
		philos[i].dead = 0;
		philos[i].meals_to_eat = input.nbr_meals;
		philos[i].last_meal = time_now();
		philos[i].printing_lock = printing_lock;
		philos[i].fork_1 = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	//	philos[i].fork_2 = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		philos[i].death_lock = death_lock;
		i++;
	}
	forks_init(philos);
}

void	forks_init(t_philo *philos)
{
	int				i;
	int				nbr_philos;
//	pthread_mutex_t	*fork;

	i = 0;
	nbr_philos = philos[0].nbr_philos;
	while (i < nbr_philos)
		pthread_mutex_init(philos[i++].fork_1, NULL);
	i = 0;
	while (i < nbr_philos)
	{
		philos[i].fork_2 = philos[(i + 1) % nbr_philos].fork_1;
		i++;
	}
	// while (i < nbr_philos)
	// {
	// 	if ((i % 2) == 0)
	// 	{
	// 		fork = philos[i].fork_2;
	// 		philos[i].fork_2 = philos[i].fork_1;
	// 		philos[i].fork_1 = fork;
	// 	}
	// 	i++;
	// }
}

void	create_threads(t_philo *philos, void *simulate_dinner)
{
	int	i;
	int	nbr_philos;
//	int	j;

	i = 0;
//	j = 1;
	nbr_philos = philos[i].nbr_philos;
	while (i < nbr_philos)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, simulate_dinner,
				(void *)&philos[i]) != 0)
			exit(1);
		pthread_detach(philos[i].philo_thread);
		i += 1;
	}
	// ft_sleep(5);
	// while (j < nbr_philos)
	// {
	// 	if (pthread_create(&philos[j].philo_thread, NULL, simulate_dinner,
	// 			(void *)&philos[j]) != 0)
	// 		exit(1);
	// 	pthread_detach(philos[j].philo_thread);
	// 	j += 2;
	// }
}

void	ft_sleep(size_t exact_time)
{
	size_t	time;

	time = time_now();
	while (time_now() < time + exact_time)
		usleep(GRANULARITY_US);
}

size_t	time_now(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * MIL_SEC_COEF) + (time.tv_usec / MIL_SEC_COEF));
}
