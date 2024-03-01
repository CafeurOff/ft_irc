/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:14:19 by lduthill          #+#    #+#             */
/*   Updated: 2024/03/01 01:47:24 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void	Server::ft_parse_buffer(std::string buffer, int client)
{
	ft_getServerName();
	if (DEBUG) // Just print the buffer and fd of client for debug
		std::cout << "[FD] --> " << client << " | " << "Buffer :" << buffer << std::endl;
		
	std::istringstream iss(buffer);
	std::string command;
	iss >> command;

    std::map<std::string, CommandFunction>::iterator it = commandFunctions.find(command);
	if (it != commandFunctions.end())
		(this->*(it->second))(buffer, client);
}

void	Server::ft_verif_pass(std::string buffer, int client)
{
	std::string pass;
	pass = buffer.substr(6, buffer.length() - 7);
	if (pass.compare(0, _password.length() + 1, _password) == 0)
	{
		std::cout << "PASSWORD GOOD" << std::endl;
		//if (client existe déjà)
//			ft_send_error(462, "PASS");
		//else
			//construct client
	}
	else
		ft_send_error(464, "Wrong Password");
	(void)client;
}

void	Server::ft_nick_receive(std::string buffer, int client)
{
	std::string	nick;
//	if (buffer.length() <= 7)
//		ft_send_error(431, "NICK");
	(void)buffer;
	(void)client;
}

void	Server::ft_user_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_quit_user(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_join_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_mode_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_topic_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_invite_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

void	Server::ft_send_error(int error, std::string command)
{
	std::string error_code;
	std::string error_message;
	std::string error_send;

	error_code = SSTR(error);
	error_message = " :";
	error_send = ":" + _servername + " " + error_code + " " + command + error_message + "\r\n";
	send(_new_socket, error_send.c_str(), error_send.length(), 0);
}


