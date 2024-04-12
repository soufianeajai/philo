#include "philo.h"

void	*supervise(void *arg)
{
	t_philo	*philo;
	int		flag;

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
