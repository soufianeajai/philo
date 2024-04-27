#include "philo.h"

void	ft_initialize(t_data *data, t_philo input, int *alloc_err)
{
	int	i;

	i = -1;
	while ((++i < input.nbr_philos) && (*alloc_err == -1))
	{
		data->philos[i].id = i + 1;
		data->philos[i].nbr_philos = input.nbr_philos;
		data->philos[i].time_to_die = input.time_to_die;
		data->philos[i].time_to_eat = input.time_to_eat;
		data->philos[i].time_to_sleep = input.time_to_sleep;
		data->philos[i].nbr_meals = input.nbr_meals;
		data->philos[i].is_full = 0;
		data->philos[i].start_time = time_now();
		data->philos[i].dead = 0;
		data->philos[i].meals_to_eat = input.nbr_meals;
		data->philos[i].last_meal = time_now();
		data->philos[i].printing_lock = &data->printing_lock;
		data->philos[i].death_lock = &data->death_lock;
		data->philos[i].meals_lock = &data->meals_lock;
		data->philos[i].is_dead = &data->is_dead;
		data->philos[i].start_dinner = &data->start_dinner;
		data->philos[i].fork_1 = malloc(sizeof(pthread_mutex_t));
		if (!data->philos[i].fork_1)
			*alloc_err = i + 1;
	}
}

void	init_mutexes(t_data *data)
{
	data->start_dinner = 0;
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->meals_lock, NULL);
	pthread_mutex_init(&data->printing_lock, NULL);
}

void	forks_init(t_philo *philos)
{
	int	i;
	int	nbr_philos;

	i = 0;
	nbr_philos = philos[0].nbr_philos;
	while (i < nbr_philos)
	{
		pthread_mutex_init(philos[i].fork_1, NULL);
		i++;
	}
	i = 0;
	while (i < nbr_philos)
	{
		philos[i].fork_2 = philos[(i + 1) % nbr_philos].fork_1;
		i++;
	}
}

void	join_threads(t_philo *philos, pthread_t *waiter)
{
	int	i;
	int	nbr_philos;

	i = 0;
	nbr_philos = philos[0].nbr_philos;
	pthread_join(*waiter, NULL);
	while (i < nbr_philos)
	{
		pthread_join(philos[i].philo_thread, NULL);
		i++;
	}
}

int handle_input_errors(int ac, char **av, t_philo *input)
{
	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) <= 0)
	{
		write(2, "Enter a valid input\n", 21);
		return(1);
	}
	if (ft_atoi(av[1]) > PTHREAD_THREADS_MAX)
	{
		printf("Thread limit (8128 threads) exceeded. Dying.\n");
		return (1);
	}	
	if (parse_input(input, av))
		return (1);
	if (input->time_to_die == 0)
	{
		printf("0 1 died\n");
		return (1);
	}
	return (0);
}
