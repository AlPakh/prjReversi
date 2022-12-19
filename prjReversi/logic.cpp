#include "Field.h"

inline void SetAttrib(int intAttrib) 
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
		default: {
			SetAttrib(15);
			cout << "00";
			break;
		}
	}
}

static void fillContainer(Bot bot, Field field) //Вывод в консоль поля 
{
	vector <Cell> inCells = field.getField();
	cout << "     1 2 3 4 5 6 7 8\n\n";
	for (int i = 1; i < 9; i++)
	{
		cout << " " << i << "   ";
		for (int j = 1; j < 9; j++)
		{
			auto foundCell = find_if(inCells.begin(), inCells.end(), [j, i](Cell cell)
				{
					return (cell.getX()==j && cell.getY()==i);
				});
			colorSymbol((*foundCell).getStatus());
		}
		SetAttrib(15);
		cout << endl;
	}
		SetAttrib(191);
		cout << endl;
		cout << string(88, ' ') << "\n";
		SetAttrib(191);
		cout << "  "; SetAttrib(32);	cout << "  ";  SetAttrib(191); cout << " —  Клетки поля, ";
		cout << "  "; SetAttrib(255);	cout << "  ";  SetAttrib(191); cout << " —  Белые  фишки, ";
		cout << "  "; SetAttrib(0);		cout << "  ";  SetAttrib(191); cout << " —  Чёрные фишки, ";
		cout << "  "; SetAttrib(44);	cout << "<>";  SetAttrib(191); cout << " —  Возможные ходы \n";
		cout << string(88, ' ') << "\n"; SetAttrib(15);
		sleep_for(1s);
}

static void PlayerShoots(CellStatus playerColor, CellStatus enemyColor, Field field) 
{
	//Сделать проверку доступных вариантов хода
	field.checkMoves(playerColor, enemyColor);

	//Обновить форму

	//Принять координату и проверить на корректность

	//Заменить необходимые фишки
}

static void GameStart(const Bot& bot, Field field) {
	cout << "\n==============================================================\n";
	cout << "Сложность - ";
	if (typeid(bot).name() == typeid(EasyBot).name()) cout << "Лёгкий\n";
	if (typeid(bot).name() == typeid(HardBot).name()) cout << "Сложный\n";
	if (typeid(bot).name() == typeid(Bot).name()) cout << "Ошибка\n";
	
	cout << "Бот - "; cout << bot.getColor();
	sleep_for(1s);

	cout << "\n\nИгра начинается...";
	sleep_for(2s);
	system("cls");

	fillContainer(bot, field);
	cout << endl;

	while (field.Exists()) 
	{
		switch(bot.getStatus())
		{
			case(1): //Бот белый - первый ход заним
			{
				//Ходит бот

				//Ходит игрок
				PlayerShoots(Black, White, field);
				break;
			}
			case(2): //Бот чёрный - первый ход за игроком
			{
				//Ходит игрок
				PlayerShoots(White, Black, field);
				//Ходит бот

				break;
			}
		}
	}
	
}