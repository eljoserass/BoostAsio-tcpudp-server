#include "../Include/Player/Player.hpp"
using namespace Server;

Player::Player(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string playerName)
{
    setPlayerName(playerName);
    setPlayerId(playerName);
}

void Player::setPlayerId(const string playerName)
{
    boost::uuids::random_generator generator;
    _id = generator();
}

boost::uuids::uuid Player::getPlayerId(void)
{
    return _id;
}

void Player::setPlayerName(const string &playerName)
{
    _playerName = playerName;
}

string Player::getPlayerName()
{
    return _playerName;
}

void Player::setIsReady()
{
    _isReady = true;
}

bool Player::getIsReady()
{
    return _isReady;
}

void Player::setPlayerInfo(const string &playerName, boost::uuids::uuid id)
{
    tuple<boost::uuids::uuid, string> playerInfo(id, playerName);
    _playerInfo = playerInfo;
}

tuple<boost::uuids::uuid, string> Player::getPlayerInfo()
{
    return _playerInfo;
}