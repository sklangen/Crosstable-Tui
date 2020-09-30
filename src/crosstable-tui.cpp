#include <ncurses.h>
#include <vector>
#include <string>
#include "Table.hpp"

#define MAX_PLAYERS 30
#define INDEX_ALPHABET L"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"

#define RESULT_NONE 0
#define RESULT_WIN 1
#define RESULT_REMIS 2
#define RESULT_LOSS 3

const float resultAsFloat[] = { 0.0f, 1.0f, 0.5f, 0.0f };
const wchar_t* resultAsWChar = L" 1½0";

struct Player {
	std::wstring name;
	std::vector<int> results;

	float score() {
		float score = 0;
		for (int& r : results) {
			score += resultAsFloat[r];
		}
		return score;
	}
};

Table* table = nullptr;
WINDOW* tableWindow = nullptr;

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

	for (size_t i = 0; i < players.size(); ++i) {
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
			p.results.push_back(RESULT_NONE);
		}
	}
}

void writeToTable() {
	for (size_t i = 0; i < players.size(); ++i) {
		int row = i + 1;
		Player& player = players[i];

		table->getCell(0, row).content = INDEX_ALPHABET[i];

		Cell &name = table->getCell(1, row);
		name.content = player.name;
		name.align = Cell::LEFT;
		name.attr = A_BOLD;

		for (size_t j = 0; j < players.size(); ++j) {
			Cell& result = table->getCell(j+2 ,row);

			if(i == j) {
				result.content = L"X";
			} else {
				result.content = resultAsWChar[player.results[j]];
				result.attr = A_BOLD;
			}
		}

		Cell &score = table->getCell(players.size()+2, row);
		score.content = std::to_wstring(player.score()).substr(0, 3);
		score.attr = A_BOLD;

		Cell &place = table->getCell(players.size()+3, row);
		place.content = std::to_wstring(i+1) + L" ";
		place.align = Cell::RIGHT;
		place.attr = A_BOLD;
	}
}


int main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	raw();
	cbreak();
	curs_set(0);

	recreateTable();
	createOrRecenterTableWindow();
	fillScores();

	int ch = 0;
	do {
		if (ch == KEY_RESIZE) {
			createOrRecenterTableWindow();
		}

		clear();
		wclear(tableWindow);

		writeToTable();
		table->draw(tableWindow);

		refresh();
		wrefresh(tableWindow);
	} while ((ch = getch()) != 'q');

	endwin();
	return 0;
}
