#include <iostream>
#include "logic.cpp"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int difficultyChoice;
    SetAttrib(15);
    cout << "Выберите сложность игры:\n  1 - Лёгкий\n  2 - Сложный\nВаш выбор: ";
    cin >> difficultyChoice;
    difficultyChoice = abs(difficultyChoice) % 2 + 1;
    cin.clear();
    Field startingField;
    switch (difficultyChoice)
    {
        case(1):
        {
            Bot* bot2 = new HardBot();
            startingField = Field(*bot2);
            Game(*bot2, startingField);
            break;
        }
        default:
        {
            Bot* bot1 = new EasyBot();
            startingField = Field(*bot1);
            Game(*bot1, startingField);
            break;
        }
    }
}