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

	Cell getCellStatusByXY(int searchedX, int searchedY) 
	{
		int a = searchedX, b = searchedY;
		 auto foundCell = find_if(this->cells.begin(), this->cells.end(), [a, b](Cell c)
			{
				return (c.getX() == a && c.getY() == b);
			});

		 return (*foundCell);
	}

	void checkMoves(CellStatus playerColor, CellStatus enemyColor)
	{
		vector<Cell> possibleCells;
		auto foundCell = for_each(this->cells.begin(), this->cells.end(), [&](Cell cell)
			{
				if (cell.getStatus() == Empty)
				{
					int cX = cell.getX(); int cY = cell.getY(); //���������� ����������� ������

					//������������ ��� �������� �� ���� ������������ (����� ����������� ������ ������ ���������)
					for  (int modY = -1; modY < 2; modY++)
					{
						for (int modX = -1; modX < 2; modX++) 
						{
							
							bool xFitsBorders = (cX + modX >= 0) && (cX + modX <= 8);	//��������� ���������� �� ������ �� ������� �������
							bool yFitsBorders = (cY + modY >= 0) && (cY + modY <= 8);	
							bool notCentralCell = !(modX == 0 && modY == 0);			//�� �������� ����������� ������� (� ��������� �� ����)
							if (xFitsBorders && yFitsBorders && notCentralCell) 
							{
								if (getCellStatusByXY(cX + modX, cY + modY).getStatus() == enemyColor) //���� ����� � ������ ������� ���� ����� ����������
								{
									//���� � ��� ����������� ���� �� ����� ������� ���� ����� ��� ����� ���������� �� ����������
									bool multiplyGO = true; int mult = 2;
									do 
									{
										int multX = cX + modX*mult; int multY = cY+modY*mult;
										bool xmFitsBorders = (multX >= 0) && (multX <= 8);	//��������� � ���������� ���������� �� ������ �� ������� �������
										bool ymFitsBorders = (multY >= 0) && (multY <= 8);
										if (getCellStatusByXY(multX, multY).getStatus() == playerColor)
										{
											possibleCells.push_back(cell);
											break; //������� ������ �� ������� ����� ��������� �����
										}
										else if (getCellStatusByXY(multX, multY).getStatus() != Empty) 
										{
											break; //����� ����������� �� ������� ����� ���������� �� ���� ������� ����� �����
										}
										mult++;
									} while(multiplyGO);
								}
							}
						}
					}

				}
			});
	}
};

void GameStart(const Bot &bot, Field field);

static void fillContainer(Bot bot, Field field);

inline void colorSymbol(CellStatus cs);