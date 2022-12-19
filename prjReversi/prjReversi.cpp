#include <iostream>
#include "logic.cpp"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Field field;
    int n;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "1 - Лёгкий\n2 - Сложный\n";
    cin >> n;
    n = abs(n) % 2 + 1;

    switch (n)
    {
        case(1):
        {
            Bot* bot2 = new HardBot();
            Field field = Field(*bot2);
            GameStart(*bot2, field);
            break;
        }
        default:
        {
            Bot* bot1 = new EasyBot();
            Field field = Field(*bot1);
            GameStart(*bot1, field);
            break;
        }
    }

}