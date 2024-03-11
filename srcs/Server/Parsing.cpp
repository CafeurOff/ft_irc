#include "../../inc/Server.hpp"

/*	ft_parse_buffer
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Parse the buffer and call the right function to handle the command
*/

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

/*	ft_verif_pass
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Verify the password of the client
*/

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

/*	ft_nick_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Receive the nickname of the client
*/

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

/*	ft_user_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Receive the username of the client
*/

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
	ft_welcome(client);
}

/*	ft_quit_user
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Quit the user from the server
*/

void	Server::ft_quit_user(std::string buffer, int client)
{
	std::string	message;
	Client		*user;

	if (buffer.find(":", 0) != std::string::npos)
		message = buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":", 0) - 2);
	else
		message = "Client Quit";
	user = findClient(client);
	if (user)
	{
		std::map<std::string, Channel>::iterator it;
		for (it = _channel.begin(); it != _channel.end(); ++it)
			it->second.quitChannel(user, message);
		_client.erase(client);
	}
	else
		ft_send_error(client, 451, "QUIT", "ERR_NOTREGISTERED");

}

/*	ft_join_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Join the client to a channel
*/

void	Server::ft_join_receive(std::string buffer, int client)
{
	std::string channel;
	std::string password;
	Channel	*chan;

	if (buffer.find("#", 0) != std::string::npos)
	{
		if (buffer.find(" ", 5) != std::string::npos)
		{
			channel = buffer.substr(6, buffer.find(" ", 5) - 6);
			password = buffer.substr(buffer.find(" ", 5), buffer.length() - buffer.find(" ", 5));
			chan = findChannel(channel);
			if (!chan)
				_channel.insert(std::pair<std::string, Channel>(channel , Channel(channel, password, findClient(client))));
			else
				chan->addUser(findClient(client), password);
		}
		else
		{
			channel = buffer.substr(6, buffer.length() - 7);
			chan = findChannel(channel);
			if (!chan)
				_channel.insert(std::pair<std::string, Channel>(channel , Channel(channel, findClient(client))));
			else
				chan->addUser(findClient(client), "");
		}
	}
}

/*	ft_topic_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Change the topic of the channel
*/

void	Server::ft_topic_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	newTopic;
	Channel	*chan;

	if (buffer.find("#", 0) == std::string::npos)
		ft_send_error(client, 461, "TOPIC", "ERR_NEEDMOREPARAMS");
	if (buffer.find(":", 0) != std::string::npos)
	{
		channel = buffer.substr(7, buffer.find(" ", 6) - 7);
		newTopic = buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":",0));
	}
	else
		channel = buffer.substr(7, buffer.length() - 8);
	chan = findChannel(channel);
	chan->topic(findClient(client), newTopic);
}

/*	ft_invite_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Invite a user to a channel
*/

void Server::ft_invite_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	user;
	Channel	*chan;
	Client	*newUser;

	if (buffer.find(" ", 0) == std::string::npos || buffer.find("#", 0) == std::string::npos || buffer.find(" ", 7) == std::string::npos)
		ft_send_error(client, 461, "INVITE", "ERR_NEEDMOREPARAMS");
	channel = buffer.substr(buffer.find("#", 0) + 1, buffer.find("\n", buffer.find("#", 0)) - (buffer.find("#", 0) + 1));
	user = buffer.substr(7, buffer.find(" ",7) - 7);
	chan = findChannel(channel);
	newUser = findClient(findFdByNickname(user));
	if (chan && newUser)
		chan->invite(findClient(client), newUser);
	else
		ft_send_error(client, 401, "INVITE", "ERR_NOSUCHNICK");
}

/*	ft_mode_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Change the mode of the client
*/

void	Server::ft_mode_receive(std::string buffer, int client)
{
	(void)buffer;
	(void)client;
}

/*	ft_kick_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Kick a user from a channel
*/

void	Server::ft_kick_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	user;
	Channel	*chan;

	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "KICK", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
	{
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
		user = buffer.substr(buffer.find(" ", 5) + 1, buffer.find(" ", buffer.find(" ", 5) + 1) - buffer.find(" ", 5) - 1);
	}
	else
	{
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
		user = buffer.substr(buffer.find(" ", 5) + 1, buffer.length() - buffer.find(" ", 5) - 2);
	}
	chan = findChannel(channel);
	chan->kick(findClient(client), user);
}


void	Server::ft_part_receive(std::string buffer, int client)
{
	std::string	channel;
	Channel	*chan;

	if (buffer.find("#", 0) == std::string::npos)
		ft_send_error(client, 461, "PART", "ERR_NEEDMOREPARAMS");
	if (buffer.find(":", 0) != std::string::npos)
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
	else
		channel = buffer.substr(6, buffer.length() - 7);
	chan = findChannel(channel);
	chan->quitChannel(findClient(client), buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":", 0) - 2));
	if (chan->getNBUser() == 0)
		_channel.erase(channel);
}

/*	ft_send_error
**	@param fd : the fd of the client
**	@param error : the error code
**	@param command : the command
**	@param type : the type of error
**	Send an error to the client
*/

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
