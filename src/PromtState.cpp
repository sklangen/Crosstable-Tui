#include "Crosstable.hpp"

PromtState::PromtState(std::wstring promt, std::wstring& input) : promt(promt), input(input) {}

void PromtState::onKeyPressed(int key) {
	switch (key) {
		case 0:
			break;
		case '\n':
			endState();
			break;
		case KEY_BACKSPACE:
			if (!input.empty()) {
				input.erase(input.length()-1, 1);
			}
			break;
		default:
			input.append(1, key);
			break;
	}
}

void PromtState::onBegin() {
	curs_set(2);
}

void PromtState::onEnd() {
	curs_set(0);
}

void PromtState::draw() {
	move(getHeight()-1, 0);
	for (int i = 0; i < getWidth(); ++i) {
		addch(' ');
	}

	move(getHeight()-1, 0);
	addwstr(promt.c_str());
	addwstr(input.c_str());
}
