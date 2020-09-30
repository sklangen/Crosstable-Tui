#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <ncurses.h>
#include <string>
#include <algorithm>
#include <cassert>

struct Cell {
	enum {
		LEFT, CENTER, RIGHT
	} align = CENTER;

	int attr = 0;
	std::wstring content = L"";

	void draw(WINDOW* win, size_t width) {
		wattron(win, attr);
		if (content.length() > width) {
			wprintw(win, "%*.*ls%lc", width-1, width-1, content.c_str(), L'…');
		} else {
			if (align == LEFT) {
				wprintw(win, "%-*.*ls", width, width, content.c_str());
			} else if (align == RIGHT) {
				wprintw(win, "%*.*ls", width, width, content.c_str());
			} else if (align == CENTER) {
				int leftpad = (width - content.length()) / 2;
				int rightpad = width - leftpad - content.length();
				wprintw(win, "%*s%ls%*s", leftpad, "", content.c_str(), rightpad, "");
			}
		}
		wattroff(win, attr);
	}
};

struct Table {
	const int width;
	const int height;

	int* widths;
	Cell* cells;

	Table(int width, int height): width(width), height(height) {

		widths = new int[width];
		cells = new Cell[width*height];

		std::fill_n(widths, width, 1);
	}

	~Table() {
		delete[] widths;
		delete[] cells;
	}

	Cell& getCell(int column, int row) {
		assert (column >= 0 && column < width);
		assert (row >= 0 && row < height);
		return cells[column + row*width];
	}

	int charWidth() {
		int w = 1;

		for (int i = 0; i < width; ++i) {
			w += widths[i] + 1;
		}

		return w;
	}

	int charHeight() {
		return 1 + 2*height;
	}

	void drawRow(WINDOW* win, int row) {
		waddwstr(win, L"│");
		for (int c = 0; c < width; ++c) {
			getCell(c, row).draw(win, widths[c]);
			waddwstr(win, L"│");
		}
		addch('\n');
	}

	void draw(WINDOW* win) {
		drawRowSeperator(win, L'┌', L'┬', L'─', L'┐');
		for (int i = 0; i < height; ++i) {
			drawRow(win, i);

			if (i < height-1) {
				drawRowSeperator(win, L'├', L'┼', L'─', L'┤');
			} else {
				drawRowSeperator(win, L'└', L'┴', L'─', L'┘');
			}
		}
	}

	void drawRowSeperator(WINDOW* win, wchar_t begin, wchar_t crossvert, wchar_t middle, wchar_t end) {
		wprintw(win, "%lc", begin);
		for (int i = 0; i < width; ++i) {
			std::wstring s(widths[i], middle);

			wchar_t c = i < width-1 ? crossvert : end;
			wprintw(win, "%ls%lc", s.c_str(), c);
		}
		addch('\n');
	}
};

#endif /* TABLE_HPP_ */
