/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:28:11 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/13 18:02:38 by lduthill         ###   ########.fr       */
/*   Updated: 2024/02/13 23:15:44 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

int	main(int ac, char **av)
{
	std::string args;

	if (ac != 3)
		return (printf("Error\n please follow this instructions\n ./ircserv <port> <password>.\n"), 1);
	args = av[1];
	if (args.length() != 4)
		return (printf("Error\n the port must be valid\n"), 1);
	for (int i = 0; args[i]; i++)
		if (!isdigit(args[i]))
			return (printf("Error\n port must be a integer\n"), 1);
	return (0);
}

