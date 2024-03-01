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