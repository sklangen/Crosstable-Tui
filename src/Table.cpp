#include "Table.hpp"

#include <algorithm>
#include <cassert>

void Cell::draw(WINDOW* win, int width) {
	wchar_t buffer[width+1];

	if ((signed) content.length() > width) {
		swprintf(buffer, width+1, L"%*.*ls%lc", width-1, width-1, content.c_str(), L'…');
	} else {
		if (align == LEFT) {
			swprintf(buffer, width+1, L"%-*.*ls", width, width, content.c_str());
		} else if (align == RIGHT) {
			swprintf(buffer, width+1, L"%*.*ls", width, width, content.c_str());
		} else if (align == CENTER) {
			int leftpad = (width - content.length()) / 2;
			int rightpad = width - leftpad - content.length();
			swprintf(buffer, width+1, L"%*s%ls%*s", leftpad, "", content.c_str(), rightpad, "");
		}
	}

	wattron(win, attr);
	waddwstr(win, buffer);
	wattroff(win, attr);
}

Table::Table(int width, int height): width(width), height(height) {
	columnWidths = new unsigned int[width];
	cells = new Cell[width*height];

	std::fill_n(columnWidths, width, 1);
}

Table::~Table() {
	delete[] columnWidths;
	delete[] cells;
}

Cell& Table::getCell(int column, int row) {
	assert (column >= 0 && column < width);
	assert (row >= 0 && row < height);
	return cells[column + row*width];
}

int Table::getCharWidth() {
	int w = 1;

	for (int i = 0; i < width; ++i) {
		w += columnWidths[i] + 1;
	}

	return w;
}

int Table::getCharHeight() {
	return 1 + 2*height;
}


void Table::draw(WINDOW* win) {
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

void Table::drawRow(WINDOW* win, int row) {
	waddwstr(win, L"│");
	for (int c = 0; c < width; ++c) {
		getCell(c, row).draw(win, columnWidths[c]);
		waddwstr(win, L"│");
	}
	addch('\n');
}

void Table::drawRowSeperator(WINDOW* win, wchar_t begin, wchar_t crossvert, wchar_t middle, wchar_t end) {
	wprintw(win, "%lc", begin);
	for (int i = 0; i < width; ++i) {
		std::wstring s(columnWidths[i], middle);

		wchar_t c = i < width-1 ? crossvert : end;
		wprintw(win, "%ls%lc", s.c_str(), c);
	}
	addch('\n');
}

