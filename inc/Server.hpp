#pragma once

#define MAX_CLIENT 20
#define DEBUG true

#include "irc.hpp"
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <ctime>
#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
	public:
		Server(char **av);
		~Server();

		/* Main */
		void	Init();
		void	Launch();

		/* Parsing and Commands */
		void	ft_parse_buffer(std::string buffer, int client);
		void	ft_verif_pass(std::string buffer, int client);
		void	ft_nick_receive(std::string buffer, int client);
		void	ft_user_receive(std::string buffer, int client);
		void	ft_quit_user(std::string buffer, int client);
		void	ft_join_receive(std::string buffer, int client);
		void	ft_mode_receive(std::string buffer, int client);
		void	ft_topic_receive(std::string buffer, int client);
		void	ft_invite_receive(std::string buffer, int client);
		void	ft_kick_receive(std::string buffer, int client);
		void	ft_privmsg(std::string buffer, int client);
		void	ft_part_receive(std::string buffer, int client);
		/* Utils */
		Channel 	*findChannel(std::string name);
		Client 		*findClient(int fd);
		std::string ft_getServerName();
		int 		findFdByNickname(const std::string& nickname);
		int 		findFd(int fd);
		int 		findChannelByName(const std::string& name);
		void		ft_welcome(int fd);

		/* Sender */
		void		ft_send_error(int fd, int error, std::string command, std::string type);
		void		SendMessage(int fd, const std::string& sender, const std::string& message);
		void 		SendMessageToChannel(const std::string& channel, Client* sender, const std::string& message);

		/*void Channel::addChannel(std::string const &name, Client* client)
		{
			Channel(name) channel1;
			if (_channel.find(name) != _channel.end())
			{
				//ERROR
				return ;
			}
			_channels.insert(std::pair(name, channel1));
			channel1.createChannel(name, client);
		}

		void Channel::addChannelPassword(std::string const &name, std::string const &password, Client* client)
		{
			Channel(name, password) channel1;
			if (_channel.find(name) != _channel.end())
			{
				//ERROR
				return ;
			}
			_channels.insert(std::pair(name, channel1));
			channel1.createChannel(name, client);
		}*/
	private:
		int		_port;
		int 	_opt;
		int		_socket;
		int		_new_socket;
		ssize_t		_valread;
		std::string	_password;
		std::string	_servername;
		typedef void (Server::*CommandFunction)(std::string, int);
		std::map<std::string, CommandFunction> commandFunctions;
		std::map<std::string, Channel> _channel;
		std::map<int , Client> _client;
};
