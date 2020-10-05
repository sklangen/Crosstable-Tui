#ifndef PLAYERSERIALIZATION_HPP_
#define PLAYERSERIALIZATION_HPP_

#include <vector>
#include <string>
#include "Player.hpp"

void writePlayers(std::vector<Player> players, std::string& filename);
std::vector<Player> readPlayers(std::string& filename);

#endif /* PLAYERSERIALIZATION_HPP_ */
