#include "../../inc/Server.hpp"

/*  SendMessage
**  Sent a message to a user
** @param fd : the file descriptor of the user
** @param sender : the nickname of the sender
** @param message : the message
*/

void Server::SendMessage(int fd, const std::string& sender, const std::string& message)
{
    Client* client = findClient(fd);
    if (client)
    {
        std::string msg = ":" + sender + " PRIVMSG " + client->getNickname() + " :" + message + "\n";
        send(fd, msg.c_str(), msg.length(), 0);
    }
}

/*  SendMessageToChannel
**  Sent a message to a channel
** @param channel : the name of the channel
** @param sender : the client who sent the message
** @param message : the message
*/

void Server::SendMessageToChannel(const std::string& channel, Client* sender, const std::string& message)
{
    std::map<std::string, Channel>::iterator it = _channel.find(channel);
    if (it != _channel.end())
        it->second.SendAllFD(":" + sender->getNickname() + " PRIVMSG #" + channel + " :" + message + "\n", sender->getFd());
}

/*  findChannelByName
**  Find the channel by the name
** @param name : the name of the channel
** @return 1 if the channel is found, -1 if not
*/

int Server::findChannelByName(const std::string& name)
{
    for (std::map<std::string, Channel>::iterator it = _channel.begin(); it != _channel.end(); ++it)
    {
        if (it->first == name)
            return 1;
    }
    return -1;
}

/*  ft_welcome
**  Sent the welcome message to the client
** @param fd : the file descriptor of the client
*/

void Server::ft_welcome(int fd)
{
    std::string welcome = findClient(fd)->getNickname() + " :Welcome to the " + ft_getServerName() + " Network, " + findClient(fd)->getNickname() + "!" + findClient(fd)->getUsername() + "@" + ft_getServerName() + "\n";
    send(fd, welcome.c_str(), welcome.length(), 0);
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

int		Server::ft_verif_empty(std::string buffer, std::string cmd, int client)
{
	if (buffer.compare(0, cmd.length(), cmd))
	{
		ft_send_error(client, 461, cmd, "ERR_NEEDMOREPARAMS");
		return (1);
	}
	return (0);
}

int Server::ft_verif_user(int client)
{
    if (findFd(client) == -1)
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return (1);
    }
    return (0);
}
