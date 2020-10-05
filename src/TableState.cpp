#include <locale>
#include <codecvt>
#include "Crosstable.hpp"
#include "PlayerSerialization.hpp"

Table* table = nullptr;
WINDOW* tableWindow = nullptr;
std::wstring nameInput = L"";
std::string lastError = "";
std::wstring loadPlayersFrom = L"";
std::wstring savePlayersTo = L"";
std::wstring lastPlayersFile = L"";
bool shallDelelePlayer = false;

using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

struct {
	int x = 1;
	int y = 0;
} cursor;

std::vector<Player> players;

void fillScores() {
	for (Player& p : players) {
		while (p.results.size() < players.size()) {
			p.results.push_back(Result::NONE);
		}
	}
}

void createOrRecenterTableWindow() {
	int x = (getWidth() - table->getCharWidth()) / 2;
	int y = (getHeight() - table->getCharHeight()) / 2;
	tableWindow = createOrMoveAndResizeWindow(tableWindow, y, x, table->getCharHeight(), table->getCharWidth());
}

void recreateTable() {
	if (table != nullptr) {
		delete table;
	}

	table = new Table(players.size()+4, players.size()+1);
	table->columnWidths[0] = 3;
	table->getCell(0, 0).content = L"#";

	table->columnWidths[1] = 32;
	table->getCell(1, 0).content = L"Name";

	for (int i = 0; i < (signed) players.size(); ++i) {
		int col = i+2;
		table->columnWidths[col] = 3;
		table->getCell(col, 0).content = INDEX_ALPHABET[i];
	}
	table->columnWidths[players.size()+2] = 7;
	table->getCell(players.size()+2, 0).content = L"Score";

	table->columnWidths[players.size()+3] = 7;
	table->getCell(players.size()+3, 0).content = L"Place";
}

void initTable() {
	fillScores();
	recreateTable();
	createOrRecenterTableWindow();
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

void changeName() {
	beginState(new PromtState(L"Enter Name: ", nameInput));
}

void addPlayer() {
	if (players.size() >= MAX_PLAYERS) {
		lastError = "Max player limit reached.";
		return;
	}

	players.push_back({ L"" });
	initTable();
	cursor = { 0, (int)players.size()-1 };
	changeName();
}

void deletePlayer() {
	beginState(new ConfirmState(L"Delete player?", shallDelelePlayer));
}

void loadPlayers() {
	loadPlayersFrom = lastPlayersFile;
	beginState(new PromtState(L"Enter filename to load from: ", loadPlayersFrom));
}

void savePlayers() {
	savePlayersTo = lastPlayersFile;
	beginState(new PromtState(L"Enter filename to save to: ", savePlayersTo));
}

void TableState::onKeyPressed(int key) {
	if (key == 0) return;

	if(!lastError.empty()) {
		lastError.erase();
	}

	if (players.empty()) {
		addPlayer();
		return;
	}

	switch (key) {
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
		case 'A':
			addPlayer();
			break;
		case 'E':
			changeName();
			break;
		case 'D':
			deletePlayer();
			break;
		case 'L':
			loadPlayers();
			break;
		case 'S':
			savePlayers();
			break;
		default:
			break;
	}
}

void fixCursorPosition() {
	if (cursor.x >= (signed) players.size()) {
		cursor.x--;
	}
	if (cursor.y >= (signed) players.size()) {
		cursor.y--;
	}
}

void removeResults() {
	for (int i = 0; i < (signed) players.size(); ++i) {
		players[i].results.erase(players[i].results.begin() + cursor.y);
	}
}

void deletePlayerIfSet() {
	if (shallDelelePlayer) {
		players.erase(players.begin() + cursor.y);
		removeResults();
		fixCursorPosition();
		initTable();
		shallDelelePlayer = false;
	}
}

void changeNameIfSet() {
	if (!nameInput.empty()) {
		players[cursor.y].name = nameInput;
		nameInput.erase();
	}
}

void loadPlayersIfSet() {
	if (!loadPlayersFrom.empty()) {
		try {
			std::string filename = converter.to_bytes(loadPlayersFrom);
			players = readPlayers(filename);
			initTable();
		} catch (std::exception& e) {
			lastError = e.what();
		}
		lastPlayersFile = loadPlayersFrom;
		loadPlayersFrom.erase();
	}
}

void savePlayersIfSet() {
	if (!savePlayersTo.empty()) {
		try {
			std::string filename = converter.to_bytes(savePlayersTo);
			writePlayers(players, filename);
		} catch (std::exception& e) {
			lastError = e.what();
		}
		lastPlayersFile = savePlayersTo;
		savePlayersTo.erase();
	}
}

void showLastErrorIfSet() {
	if (!lastError.empty()) {
		move(getHeight()-2, 0);

		int a = COLOR_PAIR(ERROR_PAIR) | A_BOLD;
		attron(a);
		addstr("Error: ");
		addstr(lastError.c_str());
		addstr("\nPress any key to continue.");
		attroff(a);
	}
}

TableState::TableState(const char*_loadPlayersFrom) {
	loadPlayersFrom = converter.from_bytes(_loadPlayersFrom);
	loadPlayersIfSet();
}

void TableState::onBegin() {
	initTable();
}

void TableState::draw() {
	clear();
	wclear(tableWindow);

	changeNameIfSet();
	deletePlayerIfSet();
	showLastErrorIfSet();
	savePlayersIfSet();
	loadPlayersIfSet();

	writeToTable();
	table->draw(tableWindow);

	refresh();
	wrefresh(tableWindow);
}
