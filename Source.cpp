#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include <time.h>
using namespace std;

enum Direction { Up = 72, Down = 80, Right = 77, Left = 75 };
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
struct Position
{
	int r;
	int c;
};
struct Snake
{
	char symbol;
	int size;
	Position* Ps;
	Direction D;
	int Score;
};
struct Food
{
	char Symbol;
	Position P;
	int Points;
};
void Initialize(Snake& S, Food& F, int Rows, int Columns)
{
	S.Score = 0;
	S.size = 5;
	S.Ps = new Position[1];
	S.D = Up;
	S.symbol = -37;
	F.Symbol = '*';
	S.Ps[0].r = Rows / 2;
	S.Ps[0].c = Columns / 2;
}
bool Valid(Snake S, Position P)
{
	for (int i = 0; i < S.size; i++)
	{
		if (P.r == S.Ps[i].r && P.c == S.Ps[i].c)
			return false;
	}
	return true;
}
Position FoodGeneration(int Rows, int Columns, Snake S)
{
	Position P;
	do
	{
		P.r = rand() % Rows;
		P.c = rand() % Columns;
	} while (!Valid(S, P));
	return P;
}
void SnakeMovement(Snake& S, int Rows, int Columns)
{
	for (int i = S.size - 1; i > 0; i--)
	{
		S.Ps[i] = S.Ps[i - 1];
	}
	switch (S.D)
	{
	case Up:
		S.Ps[0].r--;
		break;
	case Down:
		S.Ps[0].r++;
		break;
	case Right:
		S.Ps[0].c++;
		break;
	case Left:
		S.Ps[0].c--;
		break;
	}
	if (S.Ps[0].r == -1)
		S.Ps[0].r = Rows - 1;
	if (S.Ps[0].r == Rows)
		S.Ps[0].r = 0;
	if (S.Ps[0].c == -1)
		S.Ps[0].c = Columns - 1;
	if (S.Ps[0].c == Columns)
		S.Ps[0].c = 0;
}
void ShowSnake(const Snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		int r = S.Ps[i].r;
		int c = S.Ps[i].c;
		gotoRowCol(r, c);
		cout << S.symbol;
	}
}
void BlankSnake(const Snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		int r = S.Ps[i].r;
		int c = S.Ps[i].c;
		gotoRowCol(r, c);
		cout << " ";
	}
}
void DisplayFood(const Food& F)
{
	gotoRowCol(F.P.r, F.P.c);
	cout << F.Symbol;
}
int main()
{
	srand(time(0));
	int Rows = 80, Columns = 80;
	Snake S;
	Food F;
	Initialize(S, F, Rows, Columns);
	F.P = FoodGeneration(Rows, Columns, S);
	DisplayFood(F);
	S.D = Up;
	while (true)
	{
		if (_kbhit() == true)
		{
			char ch = _getch();
			if (ch == -32)
			{
				ch = _getch();
				switch (ch)
				{
				case Up:
					if (!(S.D == Down))
					{
						S.D = Up;
						break;
					}
				case Down:
					if (!(S.D == Up))
					{
						S.D = Down;
						break;
					}
				case Right:
					if (!(S.D == Left))
					{
						S.D = Right;
						break;
					}
				case Left:
					if (!(S.D == Right))
					{
						S.D = Left;
						break;
					}
				}
			}
		}
		if (S.Ps[0].r == F.P.r && S.Ps[0].c == F.P.c)
		{
			S.size++;
			S.Score++;
			F.P = FoodGeneration(Rows, Columns, S);
			DisplayFood(F);
		}
		Sleep(43);
		BlankSnake(S);
		SnakeMovement(S, Rows, Columns);
		ShowSnake(S);
		Sleep(43);
		for (int i = 1; i < S.size; i++)
		{
			if (S.Ps[0].r == S.Ps[i].r && S.Ps[0].c == S.Ps[i].c)
			{
				system("cls");
				gotoRowCol(Rows / 2, Columns / 2);
				cout << "You killed yourself!";
				gotoRowCol(Rows / 2 + 5, Columns / 2 + 5);
				cout << "Score:" << S.Score;
				return _getch();
			}
		}
	}
}