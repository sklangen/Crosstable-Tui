#include <ncurses.h>

int main(int argc, char **argv) {
	initscr();

	move(3, 3);
	addstr("Hallo Welt!");
	getch();

	endwin();
	return 0;
}
