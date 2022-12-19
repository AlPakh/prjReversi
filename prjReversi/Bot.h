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

static enum CellStatus { Empty = 0, White, Black, Possible };

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
	~Bot() {};

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
};


class EasyBot :public Bot
{
public:
	EasyBot() :Bot() {}
};

class HardBot :public Bot
{
public:
	HardBot() :Bot() {}
};