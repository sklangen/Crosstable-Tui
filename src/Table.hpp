#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <ncurses.h>
#include <string>

struct Cell {
	enum {
		LEFT, CENTER, RIGHT
	} align = CENTER;

	int attr = 0;
	std::wstring content = L"";

	void draw(WINDOW* win, int width);
};

class Table {
public:
	const int width;
	const int height;

	unsigned int* columnWidths;

	Table(int width, int height);
	~Table();
	Cell& getCell(int column, int row);
	int getCharWidth();
	int getCharHeight();
	void draw(WINDOW* win);
private:
	Cell* cells;

	void drawRow(WINDOW* win, int row);
	void drawRowSeperator(WINDOW* win, wchar_t begin, wchar_t crossvert, wchar_t middle, wchar_t end);
};

#endif /* TABLE_HPP_ */
