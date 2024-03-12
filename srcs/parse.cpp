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
	std::string pass;
	pass = buffer.substr(6, buffer.length() - 7);
	if (pass.compare(0, _password.length() + 1, _password) == 0)
	{
		//if (client existe déjà)
			//ft_send_error(462, "PASS");
		//else
			//construct client
	}
//	else if (pass.length() == 0) 
//		ft_send_error(461, "PASS");*
//	else
//		ft_send_msg(464, "PASS", "ERR_PASSWDMISMATCH");
	(void)client;
}

void	Server::ft_nick_receive(std::string buffer, int client)
{
	std::string	nick;
	nick = buffer.substr(5, buffer.length() - 6);
//	if (don't find a user with the same fd)
//		ft_send_msg(464, "PASS", "ERR_PASSWDMISMATCH");
//	if (buffer.length() <= 7)
		//ft_send_error(431, "NICK");
//	if (nick.find_first_of("*:@,!? ", 0) != std::string::npos)
//		ft_send_error(432, "NICK");
//	if (find a user with the same name)
//		ft_send_error(433, "NICK");
//	else
//		ft_nick();
	(void)buffer;
	(void)client;
}

void	Server::ft_user_receive(std::string buffer, int client)
{
	std::string	user;
	user = buffer.substr(5, buffer.length() - 6);
	user.erase(user.find_first_of(" ", 0), user.length());
	//	if(client existe)
//		ft_user();
	(void)buffer;
	(void)client;
}

void	Server::ft_quit_user(std::string buffer, int client)
{
	std::string msg;
	msg = buffer.substr(6, buffer.length() - 7);
	//send msg to every channel and delete user in every channel
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
void	Server::ft_send_error(int error, std::string command, std::string err_msg)
{
	std::string error_code;
	std::string error_message;
	std::string error_send;

	error_code = SSTR(error);
	error_message = " :" + err_msg;
	error_send = ":" + _servername + " " + error_code + " " + command + error_message + "\r\n";
	send(_new_socket, error_send.c_str(), error_send.length(), 0);
}*/