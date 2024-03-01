/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <lduthill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:31:00 by lduthill          #+#    #+#             */
/*   Updated: 2024/02/22 17:29:06 by lduthill         ###   ########.fr       */
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

	void	init();
	void	Launch();
	
	void	ft_parse_buffer(std::string buffer, int client);
	void	ft_verif_pass(std::string buffer, int client);
	void	ft_nick_receive(std::string buffer, int client);
	void	ft_user_receive(std::string buffer, int client);
	void	ft_quit_user(std::string buffer, int client);
	void	ft_join_receive(std::string buffer, int client);
	void	ft_mode_receive(std::string buffer, int client);
	void	ft_topic_receive(std::string buffer, int client);
	void	ft_invite_receive(std::string buffer, int client);
	void	ft_send_error(int error, std::string command, std::string err_msg);

private:

	int		_port;
	int 	_opt;
	int		_socket;
	int		_new_socket;
	int		_valread;
	std::string	_password;
};



