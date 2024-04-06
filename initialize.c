#include "philo.h"

void	ft_initialize(t_data *data, t_philo input)
{
	int	i;

	i = -1;
	init_mutexes(data);
	while (++i < input.nbr_philos)
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
	}
	forks_init(data->philos);
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

void	create_threads(t_philo *philos, void *simulate_dinner,
		pthread_t *waiter)
{
	int	i;
	int	nbr_philos;

	i = 0;
	nbr_philos = philos[i].nbr_philos;
	pthread_create(waiter, NULL, supervise, (void *)philos);
	while (i < nbr_philos)
	{
		pthread_create(&philos[i].philo_thread, NULL, simulate_dinner,
			(void *)&philos[i]);
		i = i + 2;
	}
	*philos->start_dinner = 1;
	ft_sleep(50);
	i = 1;
	while (i < nbr_philos)
	{
		pthread_create(&philos[i].philo_thread, NULL, simulate_dinner,
			(void *)&philos[i]);
		i = i + 2;
	}
	join_threads(philos, waiter);
}

void	ft_sleep(size_t exact_time)
{
	size_t	time;

	time = time_now();
	while (time_now() < time + exact_time)
		usleep(500);
}

size_t	time_now(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * MIL_SEC_COEF) + (time.tv_usec / MIL_SEC_COEF));
}
