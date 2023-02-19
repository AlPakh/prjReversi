#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <thread>
#include <cmath>
#define NOMINMAX
#include <Windows.h>
#include <random>

using namespace std;
using namespace std::this_thread;

/*static*/ enum CellStatus { Empty = 0, White, Black, Possible, Changing};

class Cell {
private:
	int X;
	int Y;
	CellStatus Status;
public:
	Cell() : X(0), Y(0), Status(CellStatus::Empty) {};
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

	Cell& operator= (Cell& otherCell) {
		Status = otherCell.getStatus();
		return *this;
	}
};

inline int getRandom()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 2);
	return dist(gen);
}

class Bot
{
private:
	CellStatus botColor;
public:
	Bot()
	{
		int guga = getRandom();
		switch (guga)
		{
			case(1):
			{
				this->botColor = CellStatus::White;
				break;
			}
			case(2):
			{
				this->botColor = CellStatus::Black;
				break;
			}
			default:
			{
				this->botColor = CellStatus::Empty;
				break;
			}
		}

	};
	~Bot(){};

	virtual std::string getColor() const { 
		switch (botColor) 
		{
			case(1): {
				return "White";
				break;
			}
			case(2): {
				return "Black";
				break;
			}
			default: {
				return "Empty";
				break;
			}
		}
	}

	virtual int getStatus() const {
		return botColor;
	}

	virtual Cell getBotMove(vector <Cell> possibleBotMoves) const 
	{ 
		return possibleBotMoves.front(); 
	};

};


class EasyBot : public Bot
{
public:
	EasyBot() :Bot() {}

	virtual Cell getBotMove(vector <Cell> possibleBotMoves) const //Получает случайную позицию из получаемого вектора ячеек и возвращает как возможный ход
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, possibleBotMoves.size()-1);
		int randomPosition = dist(gen);

		return (possibleBotMoves.at(randomPosition));
	}
};

class HardBot : public Bot
{
public:
	HardBot() :Bot() {}

	//Cell getBotMove(vector <Cell> possibleBotMoves) const
	//{
	//	// for_each;
	//	;
	//}
};