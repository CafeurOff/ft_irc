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
	if (ft_verif_empty(buffer, "PASS :", client))
		return ;
	pass = buffer.substr(6, buffer.length() - 7);
	if (pass.compare(0, _password.length() + 1, _password) == 0)
	{
		if (findFd(client) != -1)
		{
			ft_send_error(client, 462, "PASS", "ERR_ALREADYREGISTRED");
			return ;
		}
		else
			_client.insert(std::pair<int, Client>(client , Client(client)));
	}
	else if (pass.length() == 0)
	{
		ft_send_error(client, 461, "PASS", "ERR_NEEDMOREPARAMS");
		return ;
	}
	else
	{
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
		return ;
	}
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

	if (ft_verif_empty(buffer, "NICK ", client))
		return ;
	nick = buffer.substr(5, buffer.length() - 6);
	if (findFd(client) == -1)
	{
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
		return ;
	}
	if (buffer.length() <= 7)
	{
		ft_send_error(client, 431, "NICK", "ERR_NONICKNAMEGIVEN");
		return ;
	}
	if (nick.find_first_of("*:@,!? ", 0) != std::string::npos)
	{
		ft_send_error(client, 432, "NICK", "ERR_ERRONEUSNICKNAME");
		return ;
	}
	if (findFdByNickname(nick) != -1)
	{
		ft_send_error(client, 433, "NICK", "ERR_NICKNAMEINUSE");
		return ;
	}
	else
	{
		user = findClient(client);
		user->setNickname(nick);
	}
	ft_welcome(client);
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

	if (ft_verif_empty(buffer, "USER ", client))
		return ;
	username = buffer.substr(5, buffer.length() - 6);
	if (username.find_first_of(" ", 0) != std::string::npos)
		username.erase(username.find_first_of(" ", 0), username.length());
	if(findFd(client))
	{
		user = findClient(client);
		user->setUsername(username);
	}
}

/*	ft_quit_user
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Quit the user from the server
*/

void	Server::ft_quit_user(std::string buffer, int client)
{
	std::string msg;
	std::map<int, Client>::iterator it = _client.find(client);

	if (ft_verif_empty(buffer, "QUIT :", client))
		return ;
	if (it != _client.end())
	{
		msg = ":" + it->second.getNickname() + " QUIT :" + buffer.substr(6, buffer.length() - 7) + "\r\n";
		// send to all clients
		_client.erase(it);
	}
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

	if (ft_verif_empty(buffer, "JOIN ", client))
		return ;
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

	if (ft_verif_empty(buffer, "TOPIC ", client))
		return ;
	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "TOPIC", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
	{
		channel = buffer.substr(7, buffer.find(" ", 6) - 6);
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

void	Server::ft_invite_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	user;
	Channel	*chan;
	Client	*newUser;

	if (ft_verif_empty(buffer, "INVITE ", client))
		return ;
	if (buffer.find(" ", 0) == std::string::npos || buffer.find("#", 0) == std::string::npos || buffer.find(" ", 7) == std::string::npos)
	{
		ft_send_error(client, 461, "INVITE", "ERR_NEEDMOREPARAMS");
		return ;
	}
	channel = buffer.substr(buffer.find("#", 0) + 1, buffer.length() - buffer.find("#", 0) + 1);
	user = buffer.substr(7, buffer.find(" ",7) - 7);
	chan = findChannel(channel);
	newUser = findClient(findFdByNickname(user));
	if (chan && newUser)
		chan->invite(findClient(client), newUser);
}

/*	ft_mode_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Change the mode of the client
*/

void	Server::ft_mode_receive(std::string buffer, int client)
{
	std::string 	mode;
	size_t			args;
	std::string		channel;
	Channel			*chan;

	if (ft_verif_empty(buffer, "MODE ", client))
		return ;
	buffer.erase(0, buffer.find_first_of(" ", 0) + 1);
	if (buffer.empty() || buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "MODE", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find_first_of(" ", 0) == std::string::npos)
		return ;
	channel = buffer.substr(1,buffer.find_first_of(" ", 0) - 1);
	std::cout << "Chan is :" << channel << std::endl;
	chan = findChannel(channel);
	if (chan == NULL)
	{
		ft_send_error(client, 403, "MODE", "ERR_NOSUCHCHANNEL");
		return ;
	}
	if (chan->clientInChannel(findClient(client)) == 0)
	{
		ft_send_error(client, 441, "MODE", "ERR_USERNOTINCHANNEL");
		return ;
	}
	buffer.erase(0, buffer.find_first_of(" ", 0) + 1);
	std::cout << "MODE is :" << buffer << std::endl;
	/*
	if (chan->clientInChannel(findClient(client)) != 2 && buffer != "+b")
	{
		ft_send_error(client, 482, "MODE", "ERR_CHANOPRIVSNEEDED");
		return ;
	}
	Dans le cas ou le client n'a pas les droits admin,
	MAIS en faisant une cmd JOIN, konversation envoie une cmd MODE avec +b,
	donc ne pas renvoyez d'erreur à ce moment là*/
	args = ft_count_args(buffer);
	std::cout << "len:" << args << std::endl;
	if (args == 0)
	{
		std::cout << "mode len:" << buffer.length() << std::endl;
		chan->checkMode(&buffer);
	}
	else
	{
		std::string *param = new std::string[args + 1];
		for (size_t i = 0; i <= args; i++)
		{
			param[i] = buffer.substr(0, buffer.find(" ", 0));
			buffer.erase(0, buffer.find(" ", 0));
			std::cout << "mode :" << param[i] << std::endl;
		}
		chan->checkMode(param);
	}
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

	if (ft_verif_empty(buffer, "KICK ", client))
		return ;
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

	if (ft_verif_empty(buffer, "PART ", client))
		return ;
	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "PART", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
	else
		channel = buffer.substr(6, buffer.length() - 7);
	chan = findChannel(channel);
	chan->quitChannel(findClient(client), buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":", 0) - 2));
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

int		Server::ft_verif_empty(std::string buffer, std::string cmd, int client)
{
	if (buffer.length() - 1 <= cmd.length())
	{
		ft_send_error(client, 461, cmd, "ERR_NEEDMOREPARAMS");
		return (1);
	}
	return (0);
}

int		Server::ft_count_args(std::string buffer)
{
	int	count(0);

	for (size_t i = 0; i < buffer.length(); ++i) {
			if (buffer[i] == ' ') {
				count++;
			}
    }	return (count);
}