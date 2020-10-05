#include "PlayerSerialization.hpp"
#include "Convert.hpp"

#include <fstream>
#include <string.h>

void writePlayers(std::vector<Player> players, std::string& filename) {
	std::ofstream out(filename);
	if (out.fail()) {
		throw std::ios_base::failure(strerror(errno));
	}

	for (int i = 0; i < (signed) players.size(); ++i) {
		Player& p = players[i];
		out << getWcharConverter().to_bytes(p.name);
		for (int j = 0; j < (signed) p.results.size(); ++j) {
			wchar_t r[] = { i == j ? L'X' : resultAsWChar(p.results[j]), L'\0' };
			out << '|' << getWcharConverter().to_bytes(r);
		}
		out << '\n';
	}
}

std::vector<std::string> stringSplit(std::string s, std::string delim) {
	std::vector<std::string> v;

	size_t pos = 0;
	std::string token;

	while ((pos = s.find(delim)) != std::string::npos) {
	    token = s.substr(0, pos);
	    v.push_back(token);
	    s.erase(0, pos + delim.length());
	}
	v.push_back(s);

	return v;
}

Player readPlayer(std::string& line) {
	std::vector<std::string> parts = stringSplit(line, "|");
	Player p = { getWcharConverter().from_bytes(parts[0]) };

	for (int i = 1; i < (signed) parts.size(); ++i) {
		Result r = wcharAsResult(getWcharConverter().from_bytes(parts[i])[0]);
		p.results.push_back(r);
	}

	return p;
}

std::vector<Player> readPlayers(std::string& filename) {
	std::vector<Player> players;

	std::ifstream in(filename);
	if (in.fail()) {
		throw std::ios_base::failure(strerror(errno));
	}

	std::string line;
	while (std::getline(in, line)) {
		players.push_back(readPlayer(line));
	}

	return players;
}
