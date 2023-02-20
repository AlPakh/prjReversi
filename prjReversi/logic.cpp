#include "Field.h"

static void Game(const Bot& bot, Field mainField) {
	cout << "\n==============================================================\n";
	cout << "Выбранная сложность - ";
	if (typeid(bot).name() == typeid(EasyBot).name()) cout << "Лёгкий\n";
	if (typeid(bot).name() == typeid(HardBot).name()) cout << "Сложный\n";
	if (typeid(bot).name() == typeid(Bot).name()) cout << "Ошибка\n";

	cout << "Цвет фишек компьютера - "; cout << bot.getColor();				cout << '\a';

	sleep_for(1s);

	cout << "\n\nИгра начинается...";
	sleep_for(2s);
	system("cls");
	cout << endl;


	int moveFailCount = 0; //Счётчик пропущенных ходов
	//Если за итерацию цикла ход не совершил ни компьютер, ни игрок, игра завершается

	switch (bot.getStatus())
	{
		case(1): //Бот белый - первый ход заним
		{
			CellStatus playerColor = Black, enemyColor = CellStatus(bot.getStatus());
			while (mainField.gameExists())
			{
				//Ходит бот
				{
					system("cls");
					SetAttrib(191);
					cout << "     ХОДИТ КОМПЬЮТЕР     \n";
					SetAttrib(15);
					mainField.fillContainer();

					sleep_for(0.1s);

					vector <Cell> possCells = mainField.checkMoves(enemyColor, playerColor);

					if (possCells.size() > 0) {

						Cell moveCell = bot.getBotMove(possCells);

						int putX = moveCell.getX(), putY = moveCell.getY();

						auto botFoundCell = find_if(mainField.cells.begin(), mainField.cells.end(), [putX, putY](Cell& cell) //Фишки, которые мы ищем
							{
								return (cell.getX() == putX && cell.getY() == putY);
							});

						mainField.clearPossibles();
						(*botFoundCell).ChangeStatus(Changing);

						system("cls");
						cout << "     ХОДИТ КОМПЬЮТЕР     \n";
						mainField.fillContainer();
						sleep_for(0.1s);
						(*botFoundCell).ChangeStatus(enemyColor);

						system("cls");
						cout << "     ХОДИТ КОМПЬЮТЕР     \n";
						mainField.fillContainer();
						sleep_for(0.1s);

						//Замена промежуточных клеток
						{
							vector <Cell> cellsToFill;

							int cX = putX; int cY = putY; //Координаты проверяемой клетки

							//Модификаторы для проверки по всем направлениям (будут проверяться клетки вокруг выбранной)
							for (int modY = -1; modY < 2; modY++)
							{
								for (int modX = -1; modX < 2; modX++)
								{
									bool xFitsBorders = (cX + modX > 0) && (cX + modX <= 8);	//Изменённые координаты не выйдут за границы массива
									bool yFitsBorders = (cY + modY > 0) && (cY + modY <= 8);
									bool notCentralCell = !(modX == 0 && modY == 0);			//Не является центральной клеткой (её проверять не надо)

									if (xFitsBorders && yFitsBorders && notCentralCell && mainField.getCellByXY(cX + modX, cY + modY).getStatus() == playerColor)
										//Если рядом с пустой клеткой есть фишка противника
									{
										//Идти в том направлении пока не будет найдена своя фишка или фишки противника не закончатся
										bool multiplyGO = true; int mult = 1;
										do
										{
											int multX = cX + modX * mult; int multY = cY + modY * mult;
											bool xmFitsBorders = (multX > 0) && (multX <= 8);	//Изменённые С УМНОЖЕНИЕМ координаты не выйдут за границы массива
											bool ymFitsBorders = (multY > 0) && (multY <= 8);

											//Если условие выполнится, программа начнёт проверку другого направления  от клетки
											if (xmFitsBorders && ymFitsBorders)
											{
												//Найдена ячейка на которую можно поставить фишку
												if (mainField.getCellByXY(multX, multY).getStatus() == enemyColor)
												{
													mainField.fillColorForArray(cellsToFill, enemyColor);
													cellsToFill.clear();
													break;
												}
											}
											else { cellsToFill.clear();  break; }
											mult++;
											cellsToFill.push_back(mainField.getCellByXY(multX, multY));
										} while (multiplyGO);


									}
								}
							}
						}
					}
					else
					{
						mainField.clearPossibles();
						moveFailCount++;
					}
				}
				//Ходит игрок
				{
					//Сделать проверку доступных вариантов хода
					vector <Cell> possCells = mainField.checkMoves(playerColor, enemyColor);

					if (possCells.size() > 0) {

						//Обновить форму
						system("cls");
						SetAttrib(191);
						cout << "       ХОДИТ ИГРОК       \n";
						SetAttrib(15);
						mainField.fillContainer();

						cout << "Введите координаты ячейки, на которую хотите поставить фишку в формате XY (где X и Y — целые числа от 1 до 8)";

						//Принять координату и проверить на корректность
						int inCoords; bool inputCorrect = false;
						int putX, putY;
						do
						{

							cout << "\nВаш ввод: ";
							cin >> inCoords;			cin.clear();
							if (!cin.fail() && inCoords > 10 && inCoords <= 88 && ((inCoords % 10 <= 8 && inCoords % 10 > 0) && (inCoords / 10 > 0 && inCoords / 10 <= 8)))
							{
								//Получили координаты. Проверим, есть ли они среди возможных ходов
								putX = inCoords / 10;
								putY = inCoords % 10;

								auto foundCell = find_if(possCells.begin(), possCells.end(), [putX, putY](Cell cell)
									{
										return (cell.getX() == putX && cell.getY() == putY);
									});

								if (foundCell != possCells.end())
								{
									inputCorrect = true;
									cout << "\nПринято. Вы ставите фишку на ячейку с координатами X=" << putX << ", Y=" << putY;
								}
								else
								{
									cout << "\nВведённой координаты нет среди доступных ходов. Пожалуйста, повторите ввод...";
								}
							}
							else {
								cout << "\nВаш ввод был некорректен. Пожалуйста, введите данные снова...";
								cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							}

						} while (!inputCorrect);

						//Заменить необходимые фишки
						auto foundCell = find_if(mainField.cells.begin(), mainField.cells.end(), [putX, putY](Cell& cell) //Фишки, которые мы ищем
							{
								return (cell.getX() == putX && cell.getY() == putY);
							});

						(*foundCell).ChangeStatus(Changing);
						sleep_for(0.1s);
						system("cls");
						cout << "       ХОДИТ ИГРОК       \n";
						mainField.fillContainer();

						(*foundCell).ChangeStatus(playerColor);
						sleep_for(0.1s);

						system("cls");
						mainField.clearPossibles();
						cout << "       ХОДИТ ИГРОК       \n";
						mainField.fillContainer();

						sleep_for(0.1s);

						//Замена промежуточных клеток
						{
							vector <Cell> cellsToFill;

							int cX = putX; int cY = putY; //Координаты проверяемой клетки

							//Модификаторы для проверки по всем направлениям (будут проверяться клетки вокруг выбранной)
							for (int modY = -1; modY < 2; modY++)
							{
								for (int modX = -1; modX < 2; modX++)
								{
									bool xFitsBorders = (cX + modX > 0) && (cX + modX <= 8);	//Изменённые координаты не выйдут за границы массива
									bool yFitsBorders = (cY + modY > 0) && (cY + modY <= 8);
									bool notCentralCell = !(modX == 0 && modY == 0);			//Не является центральной клеткой (её проверять не надо)

									if (xFitsBorders && yFitsBorders && notCentralCell && mainField.getCellByXY(cX + modX, cY + modY).getStatus() == enemyColor)
										//Если рядом с пустой клеткой есть фишка противника
									{
										//Идти в том направлении пока не будет найдена своя фишка или фишки противника не закончатся
										bool multiplyGO = true; int mult = 1;
										do
										{
											int multX = cX + modX * mult; int multY = cY + modY * mult;
											bool xmFitsBorders = (multX > 0) && (multX <= 8);	//Изменённые С УМНОЖЕНИЕМ координаты не выйдут за границы массива
											bool ymFitsBorders = (multY > 0) && (multY <= 8);

											//Если условие выполнится, программа начнёт проверку другого направления  от клетки
											if (xmFitsBorders && ymFitsBorders)
											{
												//Найдена ячейка на которую можно поставить фишку
												if (mainField.getCellByXY(multX, multY).getStatus() == playerColor)
												{
													mainField.fillColorForArray(cellsToFill, playerColor);
													cellsToFill.clear();
													break;
												}
											}
											else { cellsToFill.clear();  break; }
											mult++;
											cellsToFill.push_back(mainField.getCellByXY(multX, multY));
										} while (multiplyGO);


									}
								}
							}
						}
					}
					else
					{
						mainField.clearPossibles();
						moveFailCount++;
					}
				}
				if (moveFailCount >= 2) {
					mainField.GameStoppedExisting();
					break;
				}
				else
				{
					moveFailCount = 0;
				}
			}
			break;
		}
		case(2): //Бот чёрный - первый ход за игроком
		{
			CellStatus playerColor = White, enemyColor = Black;
			while (mainField.gameExists())
			{
				//Ходит игрок
				{
					//Сделать проверку доступных вариантов хода
					vector <Cell> possCells = mainField.checkMoves(playerColor, enemyColor);

					if (possCells.size() > 0) {

						//Обновить форму
						system("cls");
						SetAttrib(191);
						cout << "       ХОДИТ ИГРОК       \n";
						SetAttrib(15);
						mainField.fillContainer();

						cout << "Введите координаты ячейки, на которую хотите поставить фишку в формате XY (где X и Y — целые числа от 1 до 8)";

						//Принять координату и проверить на корректность
						int inCoords; bool inputCorrect = false;
						int putX, putY;
						do
						{

							cout << "\nВаш ввод: ";
							cin >> inCoords;			cin.clear();
							if (!cin.fail() && inCoords > 10 && inCoords <= 88 && ((inCoords % 10 <= 8 && inCoords % 10 > 0) && (inCoords / 10 > 0 && inCoords / 10 <= 8)))
							{
								//Получили координаты. Проверим, есть ли они среди возможных ходов
								putX = inCoords / 10;
								putY = inCoords % 10;

								auto foundCell = find_if(possCells.begin(), possCells.end(), [putX, putY](Cell cell)
									{
										return (cell.getX() == putX && cell.getY() == putY);
									});

								if (foundCell != possCells.end())
								{
									inputCorrect = true;
									cout << "\nПринято. Вы ставите фишку на ячейку с координатами X=" << putX << ", Y=" << putY;
								}
								else
								{
									cout << "\nВведённой координаты нет среди доступных ходов. Пожалуйста, повторите ввод...";
								}
							}
							else {
								cout << "\nВаш ввод был некорректен. Пожалуйста, введите данные снова...";
								cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							}

						} while (!inputCorrect);

						//Заменить необходимые фишки
						auto foundCell = find_if(mainField.cells.begin(), mainField.cells.end(), [putX, putY](Cell& cell) //Фишки, которые мы ищем
							{
								return (cell.getX() == putX && cell.getY() == putY);
							});

						(*foundCell).ChangeStatus(Changing);
						sleep_for(0.1s);
						system("cls");
						cout << "       ХОДИТ ИГРОК       \n";
						mainField.fillContainer();

						(*foundCell).ChangeStatus(playerColor);
						sleep_for(0.1s);

						system("cls");
						mainField.clearPossibles();
						cout << "       ХОДИТ ИГРОК       \n";
						mainField.fillContainer();

						sleep_for(0.1s);

						//Замена промежуточных клеток
						{
							vector <Cell> cellsToFill;

							int cX = putX; int cY = putY; //Координаты проверяемой клетки

							//Модификаторы для проверки по всем направлениям (будут проверяться клетки вокруг выбранной)
							for (int modY = -1; modY < 2; modY++)
							{
								for (int modX = -1; modX < 2; modX++)
								{
									bool xFitsBorders = (cX + modX > 0) && (cX + modX <= 8);	//Изменённые координаты не выйдут за границы массива
									bool yFitsBorders = (cY + modY > 0) && (cY + modY <= 8);
									bool notCentralCell = !(modX == 0 && modY == 0);			//Не является центральной клеткой (её проверять не надо)

									if (xFitsBorders && yFitsBorders && notCentralCell && mainField.getCellByXY(cX + modX, cY + modY).getStatus() == enemyColor)
										//Если рядом с пустой клеткой есть фишка противника
									{
										//Идти в том направлении пока не будет найдена своя фишка или фишки противника не закончатся
										bool multiplyGO = true; int mult = 1;
										do
										{
											int multX = cX + modX * mult; int multY = cY + modY * mult;
											bool xmFitsBorders = (multX > 0) && (multX <= 8);	//Изменённые С УМНОЖЕНИЕМ координаты не выйдут за границы массива
											bool ymFitsBorders = (multY > 0) && (multY <= 8);

											//Если условие выполнится, программа начнёт проверку другого направления  от клетки
											if (xmFitsBorders && ymFitsBorders)
											{
												//Найдена ячейка на которую можно поставить фишку
												if (mainField.getCellByXY(multX, multY).getStatus() == playerColor)
												{
													mainField.fillColorForArray(cellsToFill, playerColor);
													cellsToFill.clear();
													break;
												}
											}
											else { cellsToFill.clear();  break; }
											mult++;
											cellsToFill.push_back(mainField.getCellByXY(multX, multY));
										} while (multiplyGO);


									}
								}
							}
						}
					}
					else
					{
						mainField.clearPossibles();
						moveFailCount++;
					}
				}
				//Ходит бот

				system("cls");
				SetAttrib(191);
				cout << "     ХОДИТ КОМПЬЮТЕР     \n";
				SetAttrib(15);
				mainField.fillContainer();

				sleep_for(0.1s);

				vector <Cell> possCells = mainField.checkMoves(enemyColor, playerColor);

				if (possCells.size() > 0) {
					Cell moveCell = bot.getBotMove(possCells);

					int putX = moveCell.getX(), putY = moveCell.getY();

					auto botFoundCell = find_if(mainField.cells.begin(), mainField.cells.end(), [putX, putY](Cell& cell) //Фишки, которые мы ищем
						{
							return (cell.getX() == putX && cell.getY() == putY);
						});

					mainField.clearPossibles();
					(*botFoundCell).ChangeStatus(Changing);
					system("cls");
					cout << "     ХОДИТ КОМПЬЮТЕР     \n";
					mainField.fillContainer();
					sleep_for(0.1s);
					(*botFoundCell).ChangeStatus(enemyColor);
					system("cls");
					cout << "     ХОДИТ КОМПЬЮТЕР     \n";
					mainField.fillContainer();
					sleep_for(0.1s);

					//Замена промежуточных клеток
					{
						vector <Cell> cellsToFill;

						int cX = putX; int cY = putY; //Координаты проверяемой клетки

						//Модификаторы для проверки по всем направлениям (будут проверяться клетки вокруг выбранной)
						for (int modY = -1; modY < 2; modY++)
						{
							for (int modX = -1; modX < 2; modX++)
							{
								bool xFitsBorders = (cX + modX > 0) && (cX + modX <= 8);	//Изменённые координаты не выйдут за границы массива
								bool yFitsBorders = (cY + modY > 0) && (cY + modY <= 8);
								bool notCentralCell = !(modX == 0 && modY == 0);			//Не является центральной клеткой (её проверять не надо)

								if (xFitsBorders && yFitsBorders && notCentralCell && mainField.getCellByXY(cX + modX, cY + modY).getStatus() == playerColor)
									//Если рядом с пустой клеткой есть фишка противника
								{
									//Идти в том направлении пока не будет найдена своя фишка или фишки противника не закончатся
									bool multiplyGO = true; int mult = 1;
									do
									{
										int multX = cX + modX * mult; int multY = cY + modY * mult;
										bool xmFitsBorders = (multX > 0) && (multX <= 8);	//Изменённые С УМНОЖЕНИЕМ координаты не выйдут за границы массива
										bool ymFitsBorders = (multY > 0) && (multY <= 8);

										//Если условие выполнится, программа начнёт проверку другого направления  от клетки
										if (xmFitsBorders && ymFitsBorders)
										{
											//Найдена ячейка на которую можно поставить фишку
											if (mainField.getCellByXY(multX, multY).getStatus() == enemyColor)
											{
												mainField.fillColorForArray(cellsToFill, enemyColor);
												cellsToFill.clear();
												break;
											}
										}
										else { cellsToFill.clear();  break; }
										mult++;
										cellsToFill.push_back(mainField.getCellByXY(multX, multY));
									} while (multiplyGO);


								}
							}
						}
					}
				}
				else
				{
					mainField.clearPossibles();
					moveFailCount++;
				}



				if (moveFailCount >= 2) {
					mainField.GameStoppedExisting();
					break;
				}
				else
				{
					moveFailCount = 0;
				}
			}
			break;
		}
	}

	//Подсчёт очков
	{
		SetAttrib(253);
		cout << "     КОНЕЦ  ИГРЫ     \n";
		SetAttrib(15);
		mainField.fillContainer();
		cout << "\n\n";
		int blackCount = 0, whiteCount = 0;

		auto foundCell = for_each(mainField.cells.begin(), mainField.cells.end(), [&](Cell& cell)
			{
				if (cell.getStatus() == Black)
				{
					blackCount++;
				}
				if (cell.getStatus() == White) {
					whiteCount++;
				}
			});

		if (blackCount > whiteCount) { cout << "Со счётом " << whiteCount << ":" << blackCount << " побеждают чёрные"; }
		else if (blackCount > whiteCount) { cout << "Со счётом " << whiteCount << ":" << blackCount << " побеждают белые"; }
		else if (blackCount == whiteCount) { cout << "Со счётом " << whiteCount << ":" << blackCount << " игра заканчивается ничьёй"; }
		else { cout << "Счёт:\nБелые - " << whiteCount << "\nЧёрные - " << blackCount; }
		cout << "\n\n";
		cout << "Для продолжения нажмите Enter...";	
		cin;
	}
}
	
