#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include <vector>

enum class Result : int {
	NONE = 0, WIN, REMIS, LOSS,
};

float resultAsFloat(Result r);
Result oppositeResult(Result r);
wchar_t resultAsWChar(Result r);
Result wcharAsResult(wchar_t wch);

struct Player {
	std::wstring name;
	std::vector<Result> results;

	float score();
};

#endif /* PLAYER_HPP_ */
