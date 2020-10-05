#include <Player.hpp>

float resultAsFloat(Result r) {
	static float lookup[] = { 0.0f, 1.0f, 0.5f, 0.0f };
	return lookup[(int)r];
}

Result oppositeResult(Result r) {
	static Result lookup[] = { Result::NONE, Result::LOSS, Result::REMIS, Result::WIN };
	return lookup[(int)r];
}
wchar_t resultAsWChar(Result r){
	wchar_t lookup[] = L" 1½0";
	return lookup[(int)r];
}

Result wcharAsResult(wchar_t wch) {
	switch (wch) {
		case L'1':
			return Result::WIN;
		case L'0':
			return Result::LOSS;
		case L'½':
			return Result::REMIS;
		default:
			return Result::NONE;
	}
}

float Player::score() {
	float score = 0;
	for (Result& r : results) {
		score += resultAsFloat(r);
	}
	return score;
}
