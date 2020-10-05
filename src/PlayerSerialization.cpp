#include "PlayerSerialization.hpp"

#include <fstream>
#include <string.h>

void writePlayers(std::vector<Player> players, std::string& filename) {

}

std::vector<std::wstring> stringSplit(std::wstring s, std::wstring delim) {
	std::vector<std::wstring> v;

	size_t pos = 0;
	std::wstring token;

	while ((pos = s.find(delim)) != std::string::npos) {
	    token = s.substr(0, pos);
	    v.push_back(token);
	    s.erase(0, pos + delim.length());
	}
	v.push_back(s);

	return v;
}

Player readPlayer(std::wstring& line) {
	std::vector<std::wstring> parts = stringSplit(line, L"|");
	Player p = { parts[0] };

	for (int i = 1; i < (signed) parts.size(); ++i) {
		p.results.push_back(wcharAsResult(parts[i][0]));
	}

	return p;
}

std::vector<Player> readPlayers(std::string& filename) {
	std::vector<Player> players;

	std::wifstream in(filename);
	if (in.fail()) {
		throw std::ios_base::failure(strerror(errno));
	}

	std::wstring line;
	while (std::getline(in, line)) {
		players.push_back(readPlayer(line));
	}

	return players;
}
