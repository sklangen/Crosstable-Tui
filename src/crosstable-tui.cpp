#include <ncurses.h>
#include <vector>
#include <string>

#define MAX_PLAYERS 30
#define MAX_NAME_WIDTH 32
#define INDEX_ALPHABET L"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"

#define RESULT_NONE 0
#define RESULT_WIN 1
#define RESULT_REMIS 2
#define RESULT_LOSS 3

const float resultAsScore[] = { 0.0f, 1.0f, 0.5f, 0.0f };
const wchar_t* resultAsWChar = L" 1½0";

struct Player {
	std::wstring name;
	std::vector<int> results;

	float score() {
		float score = 0;
		for (int& r : results) {
			score += resultAsScore[r];
		}
		return score;
	}
};


WINDOW* tableWindow = NULL;
std::vector<Player> players;

void fillScores() {
	for (Player& p : players) {
		while (p.results.size() < players.size()) {
			p.results.push_back(RESULT_NONE);
		}
	}
}

void addPlayer(std::wstring name) {
	players.push_back({ name });
	fillScores();
}

int tableWidth() {
	int resultWidth = (3 + 1) * players.size();
	return 3 + 1 + 34 + 1 + resultWidth + 7 + 1 + 5;
}

int tableHeight() {
	return 1 + 1 + (1 + 1) * players.size();
}

void drawHeader() {
	waddstr(tableWindow, " # |               Name               | ");
	for (size_t i = 0; i < players.size(); ++i) {
		wprintw(tableWindow, "%lc | ", INDEX_ALPHABET[i]);
	}
	waddstr(tableWindow, "Score | Rank\n");
}

void drawRow(size_t index) {
	wprintw(tableWindow, " %lc | %-32ls | ", INDEX_ALPHABET[index], players[index].name.c_str());
	for (size_t i = 0; i < players.size(); ++i) {
		int r = players[index].results[i];
		wchar_t c = index == i ? 'X' : resultAsWChar[r];
		wprintw(tableWindow, "%lc | ", c);
	}
	wprintw(tableWindow, " %1.1f  |  %2d\n", players[index].score(), index+1);
}

void drawSeperator(wchar_t c) {
	for (int i = 0; i < tableWidth(); ++i) {
		waddch(tableWindow, c);
	}
	waddch(tableWindow, '\n');
}

void createCenteredTableWindow() {
	int width, height;
	getmaxyx(stdscr, height, width);

	int x = (width - tableWidth()) / 2;
	int y = (height - tableHeight()) / 2;

	tableWindow = newwin(height - y, width - x, y, x);
}

void deleteTableWindowIfExists() {
	if (tableWindow != NULL) {
		delwin(tableWindow);
	}
}

void drawTable() {
	deleteTableWindowIfExists();
	createCenteredTableWindow();

	drawSeperator('=');
	drawHeader();
	drawSeperator('=');
	for (size_t i = 0; i < players.size(); ++i) {
		drawRow(i);
		drawSeperator('-');
	}
	wrefresh(tableWindow);
}

int main(int argc, char **argv) {
	addPlayer(L"Carlsen, Magnus");
	addPlayer(L"Caruana, Fabiano");
	addPlayer(L"Ding, Liren");
	addPlayer(L"Nepomniachtchi, Ian");
	addPlayer(L"Vachier-Lagrave, Maxime");
	addPlayer(L"Grischuk, Alexander");
	addPlayer(L"Aronian, Levon");
	addPlayer(L"So, Wesley");
	addPlayer(L"Radjabov, Teimour");
	addPlayer(L"Giri, Anish");
	addPlayer(L"Mamedyarov, Shakhriyar");
	addPlayer(L"Wang, Hao");
	addPlayer(L"Rapport, Richard");
	addPlayer(L"Dominguez Perez, Leinier");
	addPlayer(L"Anand, Viswanathan");
	addPlayer(L"Duda, Jan-Krzysztof");
	addPlayer(L"Karjakin, Sergey");
	addPlayer(L"Nakamura, Hikaru");
	addPlayer(L"Topalov, Veselin");
	addPlayer(L"Harikrishna, Pentala");
	addPlayer(L"Wei, Yi");
	addPlayer(L"Firouzja, Alireza");
	addPlayer(L"Andreikin, Dmitry");
	addPlayer(L"Vidit, Santosh Gujrathi");
	addPlayer(L"Svidler, Peter");
	addPlayer(L"Vitiugov, Nikita");
	addPlayer(L"Wojtaszek, Radoslaw");
	addPlayer(L"Artemiev, Vladislav");
	addPlayer(L"Vallejo Pons, Francisco");
	addPlayer(L"Le, Quang Liem");


	setlocale(LC_ALL, "");
	initscr();
	noecho();
	raw();
	cbreak();
	curs_set(0);

	do {
		clear();
		refresh();
		drawTable();
	} while (getch() != 'q');

	endwin();
	return 0;
}
