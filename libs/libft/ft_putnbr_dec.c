/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_dec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshingai <cshingai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:08:59 by cshingai          #+#    #+#             */
/*   Updated: 2024/12/20 19:22:16 by cshingai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_dec(unsigned int n)
{
	char	str;
	int		x;

	x = 0;
	if (n > 9)
	{
		x += ft_putnbr(n / 10);
		x += ft_putnbr(n % 10);
	}
	else
	{
		str = n + '0';
		x += write(1, &str, 1);
	}
	return (x);
}
