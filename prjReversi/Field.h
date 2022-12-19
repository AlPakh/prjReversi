#pragma once
#include "Bot.h"

class Cell {
private:
	int X;
	int Y;
	CellStatus Status;
public:
	Cell(): X(0), Y(0), Status(CellStatus::Empty) {};
	Cell(int newX, int newY, CellStatus newCS) : Cell() 
	{
		X = newX;
		Y = newY;
		Status = newCS;
	};
	~Cell() {};


	int getX() {
		return X;
	}
	int getY() {
		return Y;
	}	
	CellStatus getStatus() {
		return Status;
	}
	void ChangeStatus(CellStatus newCS) 
	{
		Status = newCS;
	};
};

template <class Type> class MultValue
{
private:
	Type Factor;
public:
	MultValue(const Type& value) : Factor(value) {}
	void operator()(Type& elem) const
	{
		elem *= Factor;
	}
};

class Field
{
private:
	Bot currentBot;
	bool GameExists;
	vector<Cell> cells;
public:
	Field() : GameExists(false) {};
	Field(Bot newBot) :Field()
	{
		Bot currentBot = newBot;
		GameExists = true;

		for (int i = 1; i < 9; i++) 
		{
			for (int j = 1; j < 9; j++) 
			{
				if((i==4 && j == i) || (i==5 && j==5)) cells.push_back(Cell(j, i, White));
				else if ((i == 4 && j == 5) || (i == 5 && j == 4)) cells.push_back(Cell(j, i, Black));
				else cells.push_back(Cell(j, i, Empty));
			}
		}
	}
	~Field(){};

	bool Exists() { return GameExists; }
	vector <Cell> getField() { return cells; }
	void checkMoves(CellStatus playerColor)
	{
		//auto foundCell = for_each(this->cells.begin(), this->cells.end(), );
	}
};

void GameStart(const Bot &bot, Field field);

static void fillContainer(Bot bot, Field field);

inline void colorSymbol(CellStatus cs);