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
		if (findFd(client) != -1)
			ft_send_error(client, 462, "PASS", "ERR_ALREADYREGISTRED");
		else
			_client.insert(std::pair<int, Client>(client , Client(client)));
	}
	else if (pass.length() == 0) 
		ft_send_error(client, 461, "PASS", "ERR_NEEDMOREPARAMS");
	else
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
}

void	Server::ft_nick_receive(std::string buffer, int client)
{
	std::string	nick;
	Client	*user;

	nick = buffer.substr(5, buffer.length() - 6);
	if (findFd(client) == -1)
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
	if (buffer.length() <= 7)
		ft_send_error(client, 431, "NICK", "ERR_NONICKNAMEGIVEN");
	if (nick.find_first_of("*:@,!? ", 0) != std::string::npos)
		ft_send_error(client, 432, "NICK", "ERR_ERRONEUSNICKNAME");
	if (findFdByNickname(nick) != -1)
		ft_send_error(client, 433, "NICK", "ERR_NICKNAMEINUSE");
	else
	{
		user = findClient(client);
		user->setNickname(nick);
	}
}

void	Server::ft_user_receive(std::string buffer, int client)
{
	std::string	username;
	Client	*user;

	username = buffer.substr(5, buffer.length() - 6);
	if (username.find_first_of(" ", 0) != std::string::npos)
		username.erase(username.find_first_of(" ", 0), username.length());
	if(findFd(client))
	{
		user = findClient(client);
		user->setUsername(username);
	}
}

void	Server::ft_quit_user(std::string buffer, int client)
{
	std::string msg;
	std::map<int, Client>::iterator it;

	msg = buffer.substr(6, buffer.length() - 7);
	//send msg to every channel and delete user in every channel
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first == client)
			_client.erase(it);
	}
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

void	Server::ft_send_error(int fd, int error, std::string command, std::string type)
{
	std::string error_code;
	std::string error_message;
	std::string error_send;

	error_code = SSTR(error);
	error_message = " :" + type;
	error_send = ":" + _servername + " " + error_code + " " + command + error_message + "\r\n";
	send(fd, error_send.c_str(), error_send.length(), 0);
}