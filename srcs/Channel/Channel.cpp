#include "../../inc/Channel.hpp"

Channel::Channel(const std::string name, Client *creator) : _name(name), _password(""), _topic(""), _inviteOnly(false),
			_restrictTopic(true), _limitUser(false), _passwordUse(false), _nUser(1), _limit(0)
{
	_regulars.insert(std::pair<std::string, Client*>(creator->getNickname() , creator));
	_operators.insert(std::pair<std::string, Client*>(creator->getNickname() , creator));

	std::string joinMsg = ":" + creator->getNickname() + "!~" + creator->getUsername()[0] + "@127.0.0.1 JOIN #" + name + "\n";
	sendMessage(creator, joinMsg);

	std::string modeMsg = ":127.0.0.1 MODE #" + name + " +nt\n"; // RPL_CHANNELMODEIS
	sendMessage(creator, modeMsg);

	std::string namesMsg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + _name + " :@" + creator->getNickname() + "\n"; // RPL_NAMREPLY
	sendMessage(creator, namesMsg);

	std::string endNamesMsg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n"; // RPL_ENDOFNAMES
	sendMessage(creator, endNamesMsg);
}

Channel::Channel(std::string name, std::string password, Client *creator) : _name(name), _password(password), _topic(""), _inviteOnly(false),
			_restrictTopic(true), _limitUser(false), _passwordUse(true), _nUser(1), _limit(0)
{
	_operators[creator->getNickname()] = creator;
	_regulars[creator->getNickname()] = creator;

	std::string joinMsg = ":" + creator->getNickname() + "!~" + creator->getUsername()[0] + "@127.0.0.1 JOIN #" + name + "\n";
	sendMessage(creator, joinMsg);

	std::string modeMsg = ":127.0.0.1 MODE #" + name + " +nt\n"; // RPL_CHANNELMODEIS
	sendMessage(creator, modeMsg);

	std::string namesMsg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + _name + " :@" + creator->getNickname() + "\n"; // RPL_NAMREPLY
	sendMessage(creator, namesMsg);

	std::string endNamesMsg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n"; // RPL_ENDOFNAMES
	sendMessage(creator, endNamesMsg);
}

Channel::~Channel()
{

}

void Channel::sendMessage(Client* client, const std::string& msg)
{
	send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM); // MSG_CONFIRM is a flag to tell the receiver that the data was received correctly
}

const std::string& Channel::getName() const
{
	return (_name);
}

void Channel::sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2)
{
	std::string message = ":127.0.0.1 " + code + " " + param1 + " ";

	if (!param2.empty())
		message += param2;
	message += "\n";
	sendMessage(client, message);
}

void Channel::sendAll(const std::string& message)
{
	for (std::map<std::string, Client*>::const_iterator it = _regulars.begin(); it != _regulars.end(); ++it)
		sendMessage(it->second, message);
}
void Channel::kick(Client* creator, const std::string& targetNickname)
{
	std::map<std::string, Client*>::iterator targetRegular = _regulars.find(targetNickname);
	std::map<std::string, Client*>::iterator targetOperator = _operators.find(targetNickname);

	if (targetRegular == _regulars.end())
		sendNumericResponse(creator, "442", creator->getNickname(), _name);
	else if (targetOperator != _operators.end())
		sendMessage(creator, ":127.0.0.1 Error :You can't kick an operator\n");
	else if (_operators.find(creator->getNickname()) == _operators.end())
		sendNumericResponse(creator, "482", creator->getNickname(), "ERR_CHANOPRIVSNEEDED"); // you doesn't have the operator right
	else
	{
		std::string kickMessage = ":" + creator->getNickname() + "!~" + creator->getUsername() + "@127.0.0.1 KICK #" + _name + ' ' + targetRegular->second->getNickname() + " :\n";
		sendAll(kickMessage);
		_regulars.erase(targetRegular);
	}
}

void Channel::addUser(Client* user, std::string password)
{
	if (password != _password && _passwordUse == true)
		sendNumericResponse(user, "475", user->getNickname(), "ERR_BADCHANNELKEY");
	else if (_limitUser == true && _limit <= _nUser - 1)
		sendNumericResponse(user, "471", user->getNickname(), "ERR_CHANNELISFULL");
	else if (_inviteOnly == false)
	{
		_regulars.insert(std::pair<std::string, Client*>(user->getNickname() , user));
		_nUser++;
		sendMessage(user, "JOIN #" + _name + "\n");
		sendNumericResponse(user, "331", user->getNickname(), "RPL_NOTOPIC");

	}
	else
		sendNumericResponse(user, "473", user->getNickname(), "ERR_INVITEONLYCHAN");
}

void Channel::removeUser(Client* user)
{
	_regulars.erase(user->getNickname());
	_nUser--;
}

void Channel::invite(Client* sender, Client* newUser)
{
	// Check if the sender is an operator of the channel
	if (_operators.find(sender->getNickname()) == _operators.end())
	{
		sendNumericResponse(sender, "482", sender->getNickname(), _name); // ERR_CHANOPRIVSNEEDED
		return;
	}

	//  Check if the target user is already on the channel
	if (_regulars.find(newUser->getNickname()) != _regulars.end())
	{
		sendNumericResponse(sender, "443", sender->getNickname(), newUser->getNickname()); // ERR_USERONCHANNEL
		return;
	}

	// sens a message to the target user asking for him to join the channel
	if (newUser != NULL)
	{
		std::string inviteMessage = ":" + sender->getNickname() + " INVITE " + newUser->getNickname() + " :" + _name + "\n";
		sendMessage(newUser, inviteMessage);
	}
	else
		sendNumericResponse(sender, "401", sender->getNickname(), newUser->getNickname()); // ERR_NOSUCHNICK
}

void Channel::topic(Client* sender, const std::string& newTopic) {
	// Check if the sender is an operator of the channel or the only person in it
	if (_restrictTopic && _operators.find(sender->getNickname()) == _operators.end()) {
		sendNumericResponse(sender, "482", sender->getNickname(), _name); // ERR_CHANOPRIVSNEEDED
		return;
	}

	if (_restrictTopic == false)
		return ;

	// If no new topic is specified, send the current topic to the sender
	if (newTopic.empty()) {
		sendNumericResponse(sender, "332", sender->getNickname(), _name); // RPL_TOPIC
		sendNumericResponse(sender, "333", sender->getNickname(), _name); // RPL_TOPICWHOTIME
		return;
	}

	// Check if the new topic is too long
	if (newTopic.size() > 80) {
		sendNumericResponse(sender, "409", sender->getNickname(), _name); // ERR_TOOLONG
		return;
	}

	// If everything is fine, change the topic and notify all users about this event
	_topic = newTopic;
	std::string topicMessage = ":" + sender->getNickname() + "!~" + sender->getUsername() + "@127.0.0.1 TOPIC #" + _name + " :" + _topic + "\n";
	sendAll(topicMessage);
}


void Channel::checkMode(std::string **mess)
{
	size_t i = 0;

	while (mess[i] != NULL)
	{
		std::string modeString = *mess[i];
		std::string paramString;
		if (modeString.size() < 2)
			continue;
		char modeSign = modeString[0];
		char modeChar = modeString[0];
		if (modeString.size() > 2)
			paramString = modeString[1];
		modifMode(modeSign, modeChar, paramString);
		i++;
	}
}

void Channel::modifMode(char modeSign, char modeChar, const std::string &param)
{
	if (modeSign == '+')
	{
		if (modeChar == 'i') //Change Invite Only sur true
		{
			if (_inviteOnly == false)
				_inviteOnly = true;
		}
		else if (modeChar == 't') //Definir les restrictions de la commande TOPIC pour les operateurs
		{
			if (_restrictTopic == false)
				_restricTopic = true;
		}
		else if (modeChar == 'k') //Definir un mot de passe
		{
			if (_passwordUse == false)
				setPassword(param);
		}
		else if (modeChar == 'o') //Donner le privilege d'operateur
		{

		}
		else if (modeChar == 'l') //Definir une limite d'utilisateur du canal
		{

		}
	}
	else if (modeSign == '-')
	{
		if (modeChar == 'i') //Change invite only on false
		{
			if (_inviteOnly == true)
				_inviteOnly = false;
		}
		else if (modeChar == 't') //Supprimer les restrictions de la commande TOPIC pour les operateurs
		{
			if (_restrictTopic == true)
				_restrictTopic = false;
		}
		else if (modeChar == 'k') //Supprimer le mot de passe
		{
			if (_passwordUse == true)
			{
				_passwordUse = false;
				_password = "";
			}
		}
		else if (modeChar == 'o') //Recevoir le privilege d'operateur
		{

		}
		else if (modeChar == 'l') //Supprimer la limite d'utilisateur du canal
		{

		}
	}
}

void Channel::setPassword(std::string param)
{
	_passwordUse = true;
	_password = param;
}

void Channel::SendAllFD(const std::string& message, int fd)
{
	for (std::map<std::string, Client*>::const_iterator it = _regulars.begin(); it != _regulars.end(); ++it)
	{
		if (it->second->getFd() != fd)
			sendMessage(it->second, message);
	}
}