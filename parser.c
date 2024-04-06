#include "philo.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i] || str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

long	ft_atoi(char *str)
{
	long	nbr;
	int		signe;

	signe = 1;
	nbr = 0;
	while (*str == ' ' || (9 <= *str && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			signe = -signe;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nbr = (*str - '0') + (10 * nbr);
		str++;
	}
	return (signe * nbr);
}

void	ft_assign(t_philo *input, long nbr, int flag)
{
	if (flag == 1)
		input->nbr_philos = nbr;
	else if (flag == 2)
		input->time_to_die = nbr;
	else if (flag == 3)
		input->time_to_eat = nbr;
	else if (flag == 4)
		input->time_to_sleep = nbr;
	else if (flag == 5)
	{
		if (nbr > 0)
			input->nbr_meals = nbr;
		else
			exit(1);
	}
	else
		input->nbr_meals = -1;
}

void	parse_input(t_philo *input, char **av)
{
	int	nbr;
	int	i;

	i = 1;
	while (av[i])
	{
		if (!ft_isdigit(av[i]))
		{
			exit(1);
		}
		nbr = ft_atoi(av[i]);
		ft_assign(input, nbr, i);
		i++;
	}
	if (i == 5)
		input->nbr_meals = -1;
}

void	ft_clear(t_data *data)
{
	int	i;
	int	nbr_philos;

	i = 0;
	nbr_philos = data->philos[0].nbr_philos;
	pthread_mutex_destroy(&data->meals_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->printing_lock);
	while (i < nbr_philos)
	{
		pthread_mutex_unlock(data->philos[i].fork_1);
		pthread_mutex_destroy(data->philos[i].fork_1);
		free(data->philos[i].fork_1);
		i++;
	}
	free(data->philos);
}
