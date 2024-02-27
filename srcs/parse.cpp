#include "../inc/irc.hpp"
#include "../inc/Server.hpp"

void	Server::ft_parse_buffer(std::string buffer, int client)
{
	std::cout << buffer << std::endl;
	if (buffer.compare(0, 5, "PASS ") == 0)
		ft_verif_pass(buffer, client);
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
	if (buffer.compare(6, _password.length(), _password) == 0)
	{
		//if (client existe déjà)
//			ft_send_error(462, "PASS");
		//else
			//construct client
	}
//	else
//		ft_send_error(461, "PASS");
}

void	Server::ft_nick_receive(std::string buffer, int client)
{
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
/*
void	Server::ft_send_error(461, "PASS")
{
	
}*/