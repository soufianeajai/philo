#include "philo.h"

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

void	*simulate_dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->start_dinner)
		{
			print_state(*philo, THINK, time_now(), philo->printing_lock);
			pthread_mutex_lock(philo->death_lock);
			if ((*philo->is_dead) || philo->is_full)
				break ;
			pthread_mutex_unlock(philo->death_lock);
			pthread_mutex_lock(philo->fork_1);
			print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
			if (philo->nbr_philos == 1)
				break ;
			pthread_mutex_lock(philo->fork_2);
			print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
			eat(philo);
			print_state(*philo, SLEEP, time_now(), philo->printing_lock);
			ft_sleep(philo->time_to_sleep);
		}
	}
	pthread_mutex_unlock(philo->death_lock);
	return (0);
}
void	*supervise(void *arg)
{
	t_philo	*philo;
	int		i;
	int		flag;

	i = 0;
	flag = 1;
	philo = (t_philo *)arg;
	pthread_mutex_lock(philo[0].meals_lock);
	if (philo[0].nbr_meals == -1)
		flag = -1;
	pthread_mutex_unlock(philo[0].meals_lock);
	while (1)
	{
		if (flag == -1)
		{
			if (!everyone_alive(philo))
				break ;
		}
		else
		{
			if (!everyone_alive(philo) || !philos_still_hungry(philo))
				break ;
		}
	}
	return (0);
}
int	everyone_alive(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].nbr_philos)
	{
		pthread_mutex_lock(philo[i].meals_lock);
		if (time_now() - philo[i].last_meal >= (size_t)philo[i].time_to_die)
		{
			pthread_mutex_unlock(philo[i].meals_lock);
			print_state(philo[i], DEAD, time_now(), philo[i].printing_lock);
			pthread_mutex_lock(philo[i].death_lock);
			*(philo[i].is_dead) = 1;
			pthread_mutex_unlock(philo[i].death_lock);
			return (0);
		}
		pthread_mutex_unlock(philo[i].meals_lock);
		i++;
	}
	return (1);
}

int	philos_still_hungry(t_philo *philo)
{
	int	i;
	int	everyone_full;

	i = 0;
	everyone_full = 0;
	while ((i < philo[0].nbr_philos))
	{
		pthread_mutex_lock(philo[i].meals_lock);
		if (philo[i].nbr_meals == 0)
		{
			pthread_mutex_lock(philo[i].death_lock);
			philo[i].is_full = 1;
			pthread_mutex_unlock(philo[i].death_lock);
			everyone_full++;
		}
		pthread_mutex_unlock(philo[i].meals_lock);
		i++;
	}
	if (everyone_full == philo[0].nbr_philos)
		return (0);
	return (1);
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
