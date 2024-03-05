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
           std::cout << "Channel found" << std::endl;
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

int Server::findChannelByName(const std::string& name)
{
    for (std::map<std::string, Channel>::iterator it = _channel.begin(); it != _channel.end(); ++it)
    {
        if (it->first == name)
            return 1;
    }
    return -1;
}