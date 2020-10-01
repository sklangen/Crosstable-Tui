#include "Crosstable.hpp"

ConfirmState::ConfirmState(std::wstring question, bool& answer) : question(question), answer(answer) {}

void ConfirmState::onBegin() {
	beginState(new PromtState(question + L" Type 'YES' for yes: ", input));
}

void ConfirmState::draw() {
	ungetch(0);
	endState();
}

void ConfirmState::onEnd() {
	answer = input == L"YES";
}
