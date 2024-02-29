#include "Channel.hpp"

Channel::Channel(const std::string& name)
        : _name(name), _password(""), _topic(""), _inviteOnly(false),
          _restrictTopic(true), _limitUser(false), _passwordUse(false),
          _nUser(0) {}


void Channel::sendMessage(Client* client, const std::string& msg) {
    send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM); // MSG_CONFIRM is a flag to tell the receiver that the data was received correctly
}

void Channel::createChannel(const std::string& name, Client* creator) {
    Channel channel(name);

    channel._operators[creator->getNickname()] = creator;
    channel._regulars[creator->getNickname()] = creator;

    std::string joinMsg = ":" + creator->getNickname() + "!~" + creator->getUsername()[0] + "@127.0.0.1 JOIN #" + name + "\n";
    sendMessage(creator, joinMsg);

    std::string modeMsg = ":127.0.0.1 MODE #" + name + " +nt\n"; // RPL_CHANNELMODEIS
    sendMessage(creator, modeMsg);

    std::string namesMsg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + channel._name + " :@" + creator->getNickname() + "\n"; // RPL_NAMREPLY
    sendMessage(creator, namesMsg);

    std::string endNamesMsg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n"; // RPL_ENDOFNAMES
    sendMessage(creator, endNamesMsg);
}

const std::string& Channel::getName() const {
    return (_name);
}

Channel::~Channel(){

}

void Channel::sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2){
    std::string message = ":127.0.0.1 " + code + " " + param1 + " ";

    if (!param2.empty()){
        message += param2;
    }
    message += "\n";
    sendMessage(client, message);
}

void Channel::sendAll(const std::string& message){
    for (std::map<std::string, Client*>::const_iterator it = _regulars.begin(); it != _regulars.end(); ++it)
    {
        sendMessage(it->second, message);
    }

    for (std::map<std::string, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        sendMessage(it->second, message);
    }
}

void Channel::kick(Client* creator, const std::string& targetNickname){
    std::map<std::string, Client*>::iterator targetRegular = _regulars.find(targetNickname);
    std::map<std::string, Client*>::iterator targetOperator = _operators.find(targetNickname);

    if (targetRegular == _regulars.end())
    {
        sendNumericResponse(creator, "442", creator->getNickname(), _name);
    }
    else if (targetOperator != _operators.end())
    {
        sendMessage(creator, ":127.0.0.1 Error :You can't kick an operator\n");
    }
    else
    {
        std::string kickMessage = ":" + creator->getNickname() + "!~" + creator->getUsername() + "@127.0.0.1 KICK #" + _name + ' ' + targetRegular->second->getNickname() + " :\n";
        sendAll(kickMessage);
        _regulars.erase(targetRegular);
    }

    if (_operators.find(creator->getNickname()) == _operators.end())
    {
        sendNumericResponse(creator, "482", creator->getNickname(), ""); // ERR_CHANOPRIVSNEEDED
    }
}

void Channel::addUser(Client* user) {
    _regulars[user->getNickname()] = user;
    _nUser++;
}

void Channel::removeUser(Client* user) {
    _regulars.erase(user->getNickname());
    _nUser--;
}

void Channel::invite(Client* sender, const std::string& targetNickname) {
    // Check if the sender is an operator of the channel
    if (_operators.find(sender->getNickname()) == _operators.end()) {
        sendNumericResponse(sender, "482", sender->getNickname(), _name); // ERR_CHANOPRIVSNEEDED
        return;
    }

    //  Check if the target user is already on the channel
    if (_regulars.find(targetNickname) != _regulars.end()) {
        sendNumericResponse(sender, "443", sender->getNickname(), targetNickname); // ERR_USERONCHANNEL
        return;
    }

    // sens a message to the target user asking for him to join the channel
    Client* targetUser = findClientByNickname(targetNickname); // Vous devez implémenter cette fonction pour rechercher un client par son surnom
    if (targetUser != nullptr) {
        std::string inviteMessage = ":" + sender->getNickname() + " INVITE " + targetNickname + " :" + _name + "\n";
        sendMessage(targetUser, inviteMessage);
    } else {
        sendNumericResponse(sender, "401", sender->getNickname(), targetNickname); // ERR_NOSUCHNICK
    }
}

