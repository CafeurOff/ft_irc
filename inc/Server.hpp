/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:31:00 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/26 23:00:50 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAX_CLIENT 20

#include "irc.hpp"

class Server
{
public:
	Server(char **av);
	~Server();

	void		init();
	void		Launch();
	std::string	ParseBuffer(char *buffer, int client);
private:

	int		_port;
	int 	_opt;
	int		_socket;
	int		_new_socket;
	int		_valread;
	std::string	_password;
};



