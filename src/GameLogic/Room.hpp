#pragma once

#include <vector>

#include "Player.hpp"

class Room
{
public:

    Room();

    ~Room();

    void bind(const std::vector<Player*>& player);

    void roundUpdate();

    void addPlayer(Player* player) { _players.push_back(player); }

    std::vector<Player*> getPlayers() { return _players; }

    static Room* createTestRoom();

private:

    std::vector<Player*> _players;

};

Room::Room()
{

}

Room::~Room()
{
    for (int i = 0; i < _players.size(); i++) delete _players[i];
}

void Room::bind(const std::vector<Player*>& player)
{
    _players = player;
}

void Room::roundUpdate()
{

}

Room* Room::createTestRoom()
{
    Room* room = new Room;
    room->_players.push_back(Player::createTestPlayer());
    return room;
}
