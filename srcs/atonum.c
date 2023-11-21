#include "../includes/philosopher.h"

unsigned long	ft_atoul_no_protect(char *s)
{
	unsigned long res;

	res = 0;
	while (*s && *s >= '0' && *s <= '9')
		res = (res * 10) + (*s++ - '0');
	if (res > 2147483647 || res < 1)
		return (-2);
	return (res);
}

int	ft_atoi_no_protect(char *s)
{
	long res;

	res = 0;
	while (*s && *s >= '0' && *s <= '9')
		res = (res * 10) + (*s++ - '0');
	if (res > 2147483647 || res < 1)
		return (-2);
	return ((int)res);
}
