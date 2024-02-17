/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:31:00 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/17 04:41:43 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"	// irc.hpp include all the necessary headers

class Server
{
public:
	Server(char **av);
	~Server();

	void	init();
private:

	int		_port;
	int 	_opt;
	int		_socket;
	int		_new_socket;
	int		_valread;
	std::string	_password;
};



