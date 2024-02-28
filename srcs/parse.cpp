/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:14:19 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/28 23:34:24 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void	Server::ft_parse_buffer(std::string buffer, int client)
{
	ft_getServerName();
	if (buffer.compare(0, 5, "PASS ") == 0)
		ft_verif_pass(buffer, client);
	else
		ft_send_error(451, "You have not registered");
	if (buffer.compare(0, 5, "NICK ") == 0)
		ft_nick_receive(buffer, client);
	if (buffer.compare(0, 5, "USER ") == 0)
		ft_user_receive(buffer, client);
	if (buffer.compare(0, 5, "QUIT ") == 0)
		ft_quit_user(buffer, client);
	if (buffer.compare(0, 5, "JOIN ") == 0)
		ft_join_receive(buffer, client);
	if (buffer.compare(0, 5, "MODE ") == 0)
		ft_mode_receive(buffer, client);
	if (buffer.compare(0, 6, "TOPIC ") == 0)
		ft_topic_receive(buffer, client);
	if (buffer.compare(0, 7, "INVITE ") == 0)
		ft_invite_receive(buffer, client);
}

void	Server::ft_verif_pass(std::string buffer, int client)
{
	std::string pass;
	std::cout << buffer << std::endl;
	pass = buffer.substr(6, buffer.length() - 7);
	std::cout << pass << pass.length() << _password << std::endl;
	if (pass.compare(0, _password.length() + 1, _password) == 0)
	{
		std::cout << "gg" <<std::endl;
		//if (client existe déjà)
//			ft_send_error(462, "PASS");
		//else
			//construct client
	}
	else
		std::cout << "not" << std::endl;
//	else
//		ft_send_error(461, "PASS");
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
	error_message = " :Unknown command";
	error_send = ":" + _servername + " " + error_code + " " + command + error_message + "\r\n";
	send(_new_socket, error_send.c_str(), error_send.length(), 0);
}
