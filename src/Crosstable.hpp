#ifndef CROSSTABLE_HPP_
#define CROSSTABLE_HPP_

#include <ncurses.h>
#include <vector>
#include <stack>
#include <string>
#include "Table.hpp"
#include "Player.hpp"

#define ERROR_PAIR 1

#define MAX_PLAYERS 30
#define INDEX_ALPHABET L"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"

class State {
public:
	virtual void onKeyPressed(int key) {}
	virtual void draw() {}
	virtual void onBegin() {}
	virtual void onEnd() {}
	virtual ~State() {}
};

class TableState : public State {
public:
	TableState() {}
	TableState(const char* loadPlayersFrom);
	void onKeyPressed(int key);
	void draw();
	void onBegin();
	virtual ~TableState() {}
};

class PromtState : public State {
public:
	std::wstring promt;
	std::wstring& input;
	PromtState(std::wstring promt, std::wstring& input);
	void onKeyPressed(int key);
	void draw();
	void onBegin();
	void onEnd();
	virtual ~PromtState() {}
};

class ConfirmState : public State {
public:
	std::wstring question;
	std::wstring input = L"";
	bool& answer;

	ConfirmState(std::wstring question, bool& answer);
	void draw();
	void onBegin();
	void onEnd();
	virtual ~ConfirmState() {}
};

WINDOW* createOrMoveAndResizeWindow(WINDOW* win, int y, int x, int h, int w);

int getWidth();
int getHeight();

void beginState(State* state);
void endState();

#endif /* CROSSTABLE_HPP_ */
