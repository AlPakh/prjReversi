#pragma once
#include "Bot.h"

inline void SetAttrib(int intAttrib) //Для метода fillContainer
{
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, intAttrib);
}

inline void colorSymbol(CellStatus cs) {

	switch (cs) {
	case(0): {
		SetAttrib(32); //Зелёный
		cout << "  ";  //cout << "██"
		break;
	}
	case(1): {
		SetAttrib(255); //Белый
		cout << "  ";
		break;
	}
	case(2): {
		SetAttrib(0);  //Черный
		cout << "  ";
		break;
	}
	case(3): {
		SetAttrib(44);  //С красной меткой 
		cout << "<>";
		break;
	}
	case(4): {
		SetAttrib(204);  //Красный (цвет меняющихся фишек) 
		cout << "<>";
		break;
	}
	default: {
		SetAttrib(15);
		cout << "00";
		break;
	}
	}
}

class Field
{
private:
	Bot currentBot;
	bool GameExists;
public:
	vector<Cell> cells;

	Field() : GameExists(false) {};
	Field(Bot newBot) :Field()
	{
		Bot currentBot = newBot;
		GameExists = true;
		
		//Создание элементов вектора cells - ячеек поля
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
	~Field() {};

	bool gameExists() { 
		return GameExists; 
	}

	void GameStoppedExisting() 
	{ 
		GameExists = false; 
	}

	vector <Cell> &getFieldCells() { return cells; }

	void clearPossibles() 
	{
		auto foundCell = for_each(cells.begin(), cells.end(), [&](Cell& cell)
			{
				if (cell.getStatus() == Possible)
				{
					cell.ChangeStatus(Empty); //После предыдущего хода заменяет все клетки, помеченные как возможные ходы, на клетки пустые
				}
			});
	}

	void fillContainer()  //Вывод в консоль поля 
	{
		SetAttrib(112);
		cout << "     1 2 3 4 5 6 7 8     \n";
		for (int i = 1; i < 9; i++)
		{
			cout << "   " << i << " ";
			for (int j = 1; j < 9; j++)
			{
				auto foundCell = find_if(cells.begin(), cells.end(), [j, i](Cell cell)
					{
						return (cell.getX() == j && cell.getY() == i);
					});
				colorSymbol((*foundCell).getStatus());
			}
			SetAttrib(112);
			cout << "    " << endl;
		}
		SetAttrib(112);
		cout << "                         " << endl;
		SetAttrib(15);
		cout << endl;
		SetAttrib(191);
		cout << string(88, ' ') << "\n";
		SetAttrib(191);
		cout << "  "; SetAttrib(32);	cout << "  ";  SetAttrib(191); cout << " —  Клетки поля, ";
		cout << "  "; SetAttrib(255);	cout << "  ";  SetAttrib(191); cout << " —  Белые  фишки, ";
		cout << "  "; SetAttrib(0);		cout << "  ";  SetAttrib(191); cout << " —  Чёрные фишки, ";
		cout << "  "; SetAttrib(44);	cout << "<>";  SetAttrib(191); cout << " —  Возможные ходы \n";
		cout << string(88, ' ') << "\n"; SetAttrib(15);

		sleep_for(0.2s);
	}

	Cell& getCellByXY(int searchedX, int searchedY) //Узнать ячейку по её координатам (для лямбда-функции)
	{
		 auto foundC = find_if(cells.begin(), cells.end(), [&](Cell &c)
			{
				return (c.getX() == searchedX && c.getY() == searchedY);
			});
		 return (*foundC);
	}

	void fillColorForArray(vector<Cell> cellsToSwap, CellStatus colorToSwap) //Заменить цвет ячеек, идентичных ячейкам из массива
	{
		for_each(cellsToSwap.begin(), cellsToSwap.end(), [&](Cell& cellNew) //Фишки, которым мы меняем цвет
			{
				auto foundCell = find_if(cells.begin(), cells.end(), [&](Cell cell) //Фишки, которые мы ищем
					{
						return (cell.getX() == cellNew.getX() && cell.getY() == cellNew.getY());
					});
				(*foundCell).ChangeStatus(Changing);

			});

		system("cls");
		cout << "\n";
		fillContainer();
		sleep_for(0.3s);

		for_each(cellsToSwap.begin(), cellsToSwap.end(), [&](Cell &cellNew) //Фишки, которым мы меняем цвет
			{
				auto foundCell = find_if(cells.begin(), cells.end(), [&](Cell cell) //Фишки, которые мы ищем
					{
						return (cell.getX() == cellNew.getX() && cell.getY() == cellNew.getY());
					});
				(*foundCell).ChangeStatus(colorToSwap);

				system("cls");
				cout << "\n";
				fillContainer();
				sleep_for(0.1s);
			});
	}

	//Возвращает вектор ячеек, на которые может поставить фишку игрок конкретного цвета
	vector <Cell> checkMoves(CellStatus player1Color, CellStatus player2Color)
	{
		vector <Cell> possibleCells;
		auto foundCell = for_each(cells.begin(), cells.end(), [&](Cell &cell)
			{
				if (cell.getStatus() == Possible) 
				{
					cell.ChangeStatus(Empty); //После предыдущего хода заменяет все клетки, помеченные как возможные ходы, на клетки пустые
				}
				if (cell.getStatus() == Empty)
				{
					int cX = cell.getX(); int cY = cell.getY(); //Координаты проверяемой клетки

					//Модификаторы для проверки по всем направлениям (будут проверяться клетки вокруг выбранной)
					for (int modY = -1; modY < 2; modY++)
					{
						for (int modX = -1; modX < 2; modX++)
						{
							bool xFitsBorders = (cX + modX > 0) && (cX + modX <= 8);	//Изменённые координаты не выйдут за границы массива
							bool yFitsBorders = (cY + modY > 0) && (cY + modY <= 8);
							bool notCentralCell = !(modX == 0 && modY == 0);			//Не является центральной клеткой (её проверять не надо)

							if (xFitsBorders && yFitsBorders && notCentralCell)
							{
								if (getCellByXY(cX + modX, cY + modY).getStatus() == player2Color) //Если рядом с пустой клеткой есть фишка противника
								{
									//Идти в том направлении пока не будет найдена своя фишка или фишки противника не закончатся
									bool multiplyGO = true; int mult = 1;
									do
									{
										int multX = cX + modX * mult; int multY = cY + modY * mult;
										bool xmFitsBorders = (multX > 0) && (multX <= 8);	//Изменённые С УМНОЖЕНИЕМ координаты не выйдут за границы массива
										bool ymFitsBorders = (multY > 0) && (multY <= 8);
										if (xmFitsBorders && ymFitsBorders)
										{
											if (getCellByXY(multX, multY).getStatus() == Empty) 
											{
												break;
											}
											else if (getCellByXY(multX, multY).getStatus() == player1Color) //Найдена ячейка на которую можно поставить фишку
											{
												possibleCells.push_back(cell);
												cell.ChangeStatus(Possible); //Меняем цвет на Possible
												break; 
											}
										}
										else { break; }
										mult++;
									} while (multiplyGO);

								}
							}
						}
					}

				}
			});

		return possibleCells;//fillColorForArray(possibleCells, Possible);
	}
};

void Game(const Bot &bot, Field field);