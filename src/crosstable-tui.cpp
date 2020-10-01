#include <ncurses.h>
#include <vector>
#include "Table.hpp"
#include "Player.hpp"

#define MAX_PLAYERS 30
#define INDEX_ALPHABET L"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"


Table* table = nullptr;
WINDOW* tableWindow = nullptr;

struct {
	int x = 1;
	int y = 0;
} cursor;

std::vector<Player> players = {
	{ L"Carlsen, Magnus" },
	{ L"Caruana, Fabiano" },
	{ L"Ding, Liren" },
	{ L"Nepomniachtchi, Ian" },
	{ L"Vachier-Lagrave, Maxime" },
	{ L"Grischuk, Alexander" },
	{ L"Aronian, Levon" },
	{ L"So, Wesley" },
	{ L"Radjabov, Teimour" },
};

void recreateTable() {
	if (table != nullptr) {
		delete table;
	}

	table = new Table (players.size()+4, players.size()+1);
	table->widths[0] = 3;
	table->getCell(0, 0).content = L"#";

	table->widths[1] = 32;
	table->getCell(1, 0).content = L"Name";

	for (int i = 0; i < (signed) players.size(); ++i) {
		int col = i+2;
		table->widths[col] = 3;
		table->getCell(col, 0).content = INDEX_ALPHABET[i];
	}
	table->widths[players.size()+2] = 7;
	table->getCell(players.size()+2, 0).content = L"Score";

	table->widths[players.size()+3] = 7;
	table->getCell(players.size()+3, 0).content = L"Place";
}

void createOrRecenterTableWindow() {
	int width, height;
	getmaxyx(stdscr, height, width);

	int x = (width - table->charWidth()) / 2;
	int y = (height- table->charHeight()) / 2;

	if (tableWindow == nullptr) {
		tableWindow = newwin(table->charHeight(), table->charWidth(), y, x);
	} else {
		wresize(tableWindow, table->charHeight(), table->charWidth());
		mvwin(tableWindow, y, x);
	}
}

void fillScores() {
	for (Player& p : players) {
		while (p.results.size() < players.size()) {
			p.results.push_back(Result::NONE);
		}
	}
}

void writeToTable() {
	for (int c = 0; c < (signed) players.size(); ++c) {
		int row = c + 1;
		Player& player = players[c];

		table->getCell(0, row).content = INDEX_ALPHABET[c];

		Cell &name = table->getCell(1, row);
		name.content = player.name;
		name.align = Cell::LEFT;
		name.attr = A_BOLD;
		if (cursor.x == c) {
			name.attr |= A_UNDERLINE;
		}

		for (int r = 0; r < (signed) players.size(); ++r) {
			Cell& result = table->getCell(r+2 ,row);

			if(c == r) {
				result.content = L"X";
			} else {
				result.content = resultAsWChar(player.results[r]);
				result.attr = A_BOLD;

				if (r == cursor.x && c == cursor.y) {
					result.attr |= A_STANDOUT;
				}
			}
		}

		Cell &score = table->getCell(players.size()+2, row);
		score.content = std::to_wstring(player.score()).substr(0, 3);
		score.attr = A_BOLD;

		Cell &place = table->getCell(players.size()+3, row);
		place.content = std::to_wstring(c+1) + L" ";
		place.align = Cell::RIGHT;
		place.attr = A_BOLD;
	}
}

inline int mod(int a, int b) {
	int ret = a % b;
	return ret >= 0 ? ret : ret + b;
}

void moveCursor(int dx, int dy) {
	do {
		cursor.x = mod(cursor.x + dx, players.size());
		cursor.y = mod(cursor.y + dy, players.size());
	} while (cursor.x == cursor.y);
}

void setResultAtCursor(Result r) {
	players[cursor.y].results[cursor.x] = r;
	players[cursor.x].results[cursor.y] = oppositeResult(r);
}

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	keypad(stdscr, true);
 	raw();
	cbreak();
	curs_set(0);

	recreateTable();
	createOrRecenterTableWindow();
	fillScores();

	bool running = true;
	int ch = 0;
	while (running) {

		switch (ch) {
			case KEY_RESIZE:
				createOrRecenterTableWindow();
				break;
			case KEY_UP:
				moveCursor(0, -1);
				break;
			case KEY_DOWN:
				moveCursor(0, 1);
				break;
			case KEY_LEFT:
				moveCursor(-1, 0);
				break;
			case KEY_RIGHT:
				moveCursor(1, 0);
				break;
			case '1':
			case 'w':
				setResultAtCursor(Result::WIN);
				break;
			case '0':
			case 'l':
				setResultAtCursor(Result::LOSS);
				break;
			case '2':
			case '5':
			case ',':
			case '.':
			case 'r':
			case 'd':
				setResultAtCursor(Result::REMIS);
				break;
			case KEY_BACKSPACE:
			case KEY_DC:
				setResultAtCursor(Result::NONE);
				break;
			default:
				break;
		}

		clear();
		wclear(tableWindow);

		writeToTable();
		table->draw(tableWindow);

		refresh();
		wrefresh(tableWindow);

		ch = getch();
	}

	endwin();
	return 0;
}
