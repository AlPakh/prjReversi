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

enum CellStatus { Empty = 0, White, Black, Possible, Changing};

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

inline int getRandom(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(a, b);
	return dist(gen);
}

class Bot
{
private:
	CellStatus botColor;
public:
	Bot()
	{
		int guga = getRandom(1,2);
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

	//Получает случайную позицию из получаемого вектора ячеек и возвращает как возможный ход
	virtual Cell getBotMove(vector <Cell> possibleBotMoves) const 
	{
		int randomPosition = getRandom(0, possibleBotMoves.size() - 1);
		return (possibleBotMoves.at(randomPosition));
	}
};

class HardBot : public Bot
{
public:
	HardBot() :Bot() {}

	//Получает случайную позицию из получаемого вектора ячеек и возвращает как возможный ход
	virtual Cell getBotMove(vector <Cell> possibleBotMoves) const 
	{
		//В первую очередь, проверять наличие угловых клеток среди возможных ходов
		vector <Cell> cornerCells;
		auto crnCell = for_each(possibleBotMoves.begin(), possibleBotMoves.end(), [&](Cell& cell)
			{
				if ((cell.getX() == 1 && cell.getY() == 1) ||
					(cell.getX() == 1 && cell.getY() == 8) ||
					(cell.getX() == 8 && cell.getY() == 1) ||
					(cell.getX() == 8 && cell.getY() == 8))
				{
					cornerCells.push_back(cell);
				}
			});
		if (cornerCells.size() != 0) 
		{
			int randomPosition = getRandom(0, cornerCells.size() - 1);
			return (cornerCells.at(randomPosition));
		}

		//Во вторую очередь, проверять наличие боковых клеток среди возможных ходов
		vector <Cell> sideCells;
		auto sdCell = for_each(possibleBotMoves.begin(), possibleBotMoves.end(), [&](Cell& cell)
			{
				if (cell.getX() == 1 || cell.getY() == 1 || cell.getX() == 8 || cell.getY() == 8)
				{
					sideCells.push_back(cell);
				}
			});
		if (sideCells.size() != 0)
		{
			int randomPosition = getRandom(0, sideCells.size() - 1);
			return (sideCells.at(randomPosition));
		}

		//Иначе просто выбрать ход среди возможных случайно
		int randomPosition = getRandom(0, possibleBotMoves.size() - 1);
		return (possibleBotMoves.at(randomPosition));
	}
};