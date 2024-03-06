#include "../../inc/Server.hpp"

/*  ft_privmsg
**  Sent a private message to a user or a channel
** @param buffer : the message
** @param client : the client who sent the message
*/

void	Server::ft_privmsg(std::string buffer, int client)
{
    Client *user;
    std::string receiver;
    std::string message;
    std::string channel;

    if (buffer.find("#", 0) != std::string::npos)
    {
        channel = buffer.substr(9, buffer.find(" ", 0) - 10);
        if (channel.find_first_of(" ", 0) != std::string::npos)
            channel.erase(channel.find_first_of(" ", 0), channel.length());
    }
    message = std::string(buffer.begin() + buffer.find(":", 0) + 1, buffer.end());
    receiver = buffer.substr(8, buffer.length() - 9);
    if (receiver.find_first_of(" ", 0) != std::string::npos)
		receiver.erase(receiver.find_first_of(" ", 0), receiver .length());
    user = findClient(client);

    if (channel != "")
    {
        if (findChannelByName(channel) == -1)
            ft_send_error(client ,401, "ERROR", "ERR_NOSUCHCHANNEL");
        else
            SendMessageToChannel(channel, user, message);
    }
    else
    {
        if (findFdByNickname(receiver) == -1)
            ft_send_error(client ,401, "ERROR", "ERR_NOSUCHNICK");
        SendMessage(findFdByNickname(receiver), user->getNickname(), message);
    }
}

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
