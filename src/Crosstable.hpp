#ifndef CROSSTABLE_HPP_
#define CROSSTABLE_HPP_

#include <ncurses.h>
#include <vector>
#include <stack>
#include "Table.hpp"
#include "Player.hpp"

#define MAX_PLAYERS 30
#define INDEX_ALPHABET L"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"

class State {
public:
	virtual void onKeyPressed(int key) = 0;
	virtual void draw() {}
	virtual void onBegin() {}
	virtual void onEnd() {}
	virtual ~State() {}
};

class MainState : public State {
public:
	void onKeyPressed(int key);
	void draw();
	void onBegin();
	virtual ~MainState() {}
};

void beginState(State* state);
void endState();

#endif /* CROSSTABLE_HPP_ */
