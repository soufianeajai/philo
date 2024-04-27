#include "philo.h"

void	*simulate_dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2)
		ft_sleep(50);
	if (philo->start_dinner)
	{
		while (!check_simulation(philo, 1))
		{
			print_state(*philo, THINK, time_now(), philo->printing_lock);
			pthread_mutex_lock(philo->fork_1);
			print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
			if (check_simulation(philo, 0))
				break ;
			pthread_mutex_lock(philo->fork_2);
			print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
			eat(philo);
			print_state(*philo, SLEEP, time_now(), philo->printing_lock);
			ft_sleep(philo->time_to_sleep);
		}
	}
	return (0);
}
void	eat(t_philo *philo)
{
	print_state(*philo, EAT, time_now(), philo->printing_lock);
	pthread_mutex_lock(philo->meals_lock);
	philo->last_meal = time_now();
	philo->nbr_meals--;
	pthread_mutex_unlock(philo->meals_lock);
	ft_sleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->fork_2);
	pthread_mutex_unlock(philo->fork_1);
}

int	check_simulation(t_philo *philo, int flag)
{
	if (flag == 1)
	{
		pthread_mutex_lock(philo->death_lock);
		if (*philo->is_dead || philo->is_full)
		{
			pthread_mutex_unlock(philo->death_lock);
			return (1);
		}
		pthread_mutex_unlock(philo->death_lock);
	}
	else
	{
		if (philo->nbr_philos == 1)
		{
			pthread_mutex_unlock(philo->fork_1);
			return (1);
		}
	}
	return (0);
}

void	print_state(t_philo philo, char *state, size_t time,
		pthread_mutex_t *lock)
{
	pthread_mutex_lock(philo.death_lock);
	if (*philo.is_dead == 1)
	{
		pthread_mutex_unlock(philo.death_lock);
		return ;
	}
	pthread_mutex_unlock(philo.death_lock);
	pthread_mutex_lock(lock);
	printf("%lu %d %s\n", (time - philo.start_time), philo.id, state);
	pthread_mutex_unlock(lock);
}
int	create_threads(t_philo *philos, void *simulate_dinner,
		pthread_t *waiter)
{
	int	i;
	int	nbr_philos;

	i = 0;
	nbr_philos = philos[i].nbr_philos;
	pthread_create(waiter, NULL, supervise, (void *)philos);
	while (i < nbr_philos)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, simulate_dinner,
			(void *)&philos[i]))
		{
			printf("Error in creating threads");
			return (1);
		}
		i++;
	}
	*philos->start_dinner = 1;
	join_threads(philos, waiter);
	return (0);
}