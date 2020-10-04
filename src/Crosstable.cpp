#include "Crosstable.hpp"

std::stack<State*> states;

struct {
	int width, height;
} screenSize;

void beginState(State* state) {
	states.push(state);
	state->onBegin();
	ungetch(0);
}

void endState() {
	states.top()->onEnd();
	delete states.top();
	states.pop();
}

void readScreenSize() {
	getmaxyx(stdscr, screenSize.height, screenSize.width);
}

int getWidth() {
	return screenSize.width;
}

int getHeight() {
	return screenSize.height;
}

WINDOW* createOrMoveAndResizeWindow(WINDOW* win, int y, int x, int h, int w) {
	if (win == nullptr) {
		win = newwin(h, w, y, x);
	} else {
		wresize(win, h, w);
		mvwin(win, y, x);
	}
	return win;
}

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	keypad(stdscr, true);
 	raw();
	cbreak();
	curs_set(0);

	start_color();
	init_pair(ERROR_PAIR, COLOR_WHITE, COLOR_RED);

	readScreenSize();
	beginState(new MainState());

	wint_t key = 0;
	while (!states.empty()) {
		if (key == KEY_RESIZE) {
			readScreenSize();
		}

		states.top()->onKeyPressed(key);

		states.top()->draw();

		get_wch(&key);
	}

	endwin();
	return 0;
}
