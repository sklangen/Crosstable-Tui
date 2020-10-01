#include "Crosstable.hpp"

std::stack<State*> states;

void beginState(State* state) {
	states.push(state);
	state->onBegin();
}

void endState() {
	states.top()->onEnd();
	delete states.top();
	states.pop();
}

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	keypad(stdscr, true);
 	raw();
	cbreak();
	curs_set(0);

	beginState(new MainState());

	while (!states.empty()) {
		states.top()->draw();
		states.top()->onKeyPressed(getch());
	}

	endwin();
	return 0;
}
