#include "philo.h"

void	*simulate_dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->death_lock);
		if (philo->dead || !philo->nbr_meals)
			break ;
		pthread_mutex_unlock(philo->death_lock);
		// if (philo->id % 2 == 0)
		// {
		// 	print_state(*philo, THINK, time_now(), philo->printing_lock);
		// 	ft_sleep(philo->time_to_eat - 20);
		// }
		// else
		print_state(*philo, THINK, time_now(), philo->printing_lock);
		pthread_mutex_lock(philo->fork_1);
		print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
		pthread_mutex_lock(philo->fork_2);
		print_state(*philo, TAKE_FORK, time_now(), philo->printing_lock);
		print_state(*philo, EAT, time_now(), philo->printing_lock);
		pthread_mutex_lock(philo->death_lock);
		philo->last_meal = time_now();
		philo->nbr_meals--;
		pthread_mutex_unlock(philo->death_lock);
		ft_sleep(philo->time_to_eat);
		pthread_mutex_unlock(philo->fork_2);
		pthread_mutex_unlock(philo->fork_1);
		print_state(*philo, SLEEP, time_now(), philo->printing_lock);
		ft_sleep(philo->time_to_sleep);
	}
	pthread_mutex_unlock(philo->death_lock);
	return (0);
}

int	everyone_alive(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].nbr_philos)
	{
		pthread_mutex_lock(philo[i].death_lock);
		if (time_now() - philo[i].last_meal >= (size_t)philo[i].time_to_die)
		{
			pthread_mutex_unlock(philo[i].death_lock);
			print_state(philo[i], DEAD, time_now(), philo[i].printing_lock);
			i = 0;
			pthread_mutex_lock(philo[0].death_lock);
			while (i < philo[0].nbr_philos)
			{
	//		printf("All philosophers are dead\n");
				philo[i].dead = 1;
				pthread_mutex_unlock(philo[i].fork_1);
				pthread_mutex_destroy(philo[i].fork_1);
				i++;
			}
			pthread_mutex_unlock(philo[0].death_lock);
		//	pthread_mutex_unlock(philo[i].death_lock);
			pthread_mutex_destroy(philo->printing_lock);
			return (0);
		}
		pthread_mutex_unlock(philo[i].death_lock);
		i++;
	}
	return (1);
}

int	philos_still_hungry(t_philo *philo)
{
	int	i;
	int	need_to_eat;

	i = 0;
	need_to_eat = 0;
	pthread_mutex_lock(philo[0].death_lock);
	while ((i < philo[0].nbr_philos) && (philo[0].nbr_meals > 0))
	{
		need_to_eat += philo[i].meals_to_eat;
		i++;
	}
	if ((need_to_eat == 0) && (philo[0].nbr_meals >= 0))
	{
		pthread_mutex_unlock(philo[0].death_lock);
		return (0);
	}
	pthread_mutex_unlock(philo[0].death_lock);
	return (1);
}

void	print_state(t_philo philo, char *state, size_t time,
		pthread_mutex_t *lock)
{
	if (philo.dead == 1)
		return ;
	pthread_mutex_lock(lock);
	printf("%lu %d %s\n", (time - philo.start_time), philo.id, state);
	if (strncmp(state, DEAD, strlen(DEAD)) != 0)
		pthread_mutex_unlock(lock);
}
// do not change anything in this function !!!!!~!
