/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduthill <lduthill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:47:06 by lduthill          #+#    #+#             */
/*   Updated: 2024/03/04 15:42:49 by lduthill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        channel = buffer.substr(8, buffer.find(" ", 0) - 8);
        if (channel.find_first_of(" ", 0) != std::string::npos)
            channel.erase(channel.find_first_of(" ", 0), channel.length());
    }
    message = std::string(buffer.begin() + buffer.find(":", 0) + 1, buffer.end());
    if (message.find_first_of(" ", 0) != std::string::npos)
        message.erase(message.find_first_of(" ", 0), message.length());
    receiver = buffer.substr(8, buffer.length() - 9);
    if (receiver.find_first_of(" ", 0) != std::string::npos)
		receiver.erase(receiver.find_first_of(" ", 0), receiver .length());
    user = findClient(client);

    if (channel != "")
    {
        std::cout << "channel" << std::endl;
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

void    Server::SendMessage(int fd, std::string sender, std::string message)
{
    std::string msg;

    msg = "PRIVMSG " + sender + " :" + message + "\n";
    send(fd, msg.c_str(), msg.length(), 0);
}