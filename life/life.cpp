// life.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h> 
#include <iostream> 
#include <array> 
#include <ctime> 
#include <conio.h> 

typedef unsigned char BYTE;

using std::array;

template<size_t I, size_t J, typename T>
void Generate(array<array<T, J>, I> &arr);

template<size_t I, size_t J, typename T>
void UpdateGeneration(array<array<T, J>, I> &arr, array<array<T, J>, I> &oldarr);

template<size_t I, size_t J, typename T>
BYTE GetCell(BYTE x, BYTE y, array<array<T, J>, I> const &arr);

template<size_t I, size_t J, typename T>
BYTE GetCellsCount(BYTE x, BYTE y, array<array<T, J>, I> const &arr);

template<size_t I, size_t J, typename T>
int sumOrganism(array<array<T, J>, I> const &arr);

template<size_t I, size_t J, typename T>
void print(const HANDLE &StdOut, const COORD &Coord, array<array<T, J>, I> const &arr);


int main(void) {
	srand(time(NULL));
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord{ 0, 0 };

	array<array<bool, 60>, 20> Generation{ false };
	array<array<bool, 60>, 20> oldGeneration{ false };
	int sum{};

	Generate(Generation);

	do {
		UpdateGeneration(Generation, oldGeneration);
		print(hStdOut, coord, Generation);
		sum = sumOrganism(Generation);
		if (_kbhit() && (_getch() == 27)) return 0;
		if (Generation == oldGeneration) break;
		Sleep(100);
	} while (sum);

	if (!sum) {
		system("cls");
		std::cout << "All organisms were lost!" << std::endl;
	}
	else {
		std::cout << "\nFinish!" << std::endl;
	}
	system("pause");
	return 0;
}


template<size_t I, size_t J, typename T>
void Generate(array<array<T, J>, I> &arr) {
	for (auto &i : arr) {
		for (auto &j : i) {
			j = rand() % 2;
		}
	}
}


template<size_t I, size_t J, typename T>
void UpdateGeneration(array<array<T, J>, I> &arr, array<array<T, J>, I> &oldarr) {
	BYTE cellsCount{};
	oldarr = arr;
	for (int i{}; i < I; i++) {
		for (int j{}; j < J; j++) {
			cellsCount = GetCellsCount(i, j, oldarr);
			if (!oldarr[i][j]) {
				if (cellsCount == static_cast<BYTE>(3)) arr[i][j] = true;
			}
			else {
				if ((cellsCount < static_cast<BYTE>(2)) || (cellsCount > static_cast<BYTE>(3))) arr[i][j] = false;
			}
		}
	}
}


template<size_t I, size_t J, typename T>
BYTE GetCell(BYTE x, BYTE y, array<array<T, J>, I> const &arr) {
	if ((x > I - 1) || (y > J - 1)) {
		return 0;
	}
	return static_cast<BYTE>(arr[x][y]);
}


template<size_t I, size_t J, typename T>
BYTE GetCellsCount(BYTE x, BYTE y, array<array<T, J>, I> const &arr) {
	BYTE Result{};
	Result += GetCell(x - 1, y, arr);
	Result += GetCell(x - 1, y - 1, arr);
	Result += GetCell(x, y - 1, arr);
	Result += GetCell(x + 1, y - 1, arr);
	Result += GetCell(x + 1, y, arr);
	Result += GetCell(x + 1, y + 1, arr);
	Result += GetCell(x, y + 1, arr);
	Result += GetCell(x - 1, y + 1, arr);
	return Result;
}


template<size_t I, size_t J, typename T>
int sumOrganism(array<array<T, J>, I> const &arr) {
	int Result{};
	for (auto const i : arr) {
		for (auto const j : i) {
			Result += j;
		}
	}
	return Result;
}


template<size_t I, size_t J, typename T>
void print(const HANDLE &StdOut, const COORD &Coord, array<array<T, J>, I> const &arr) {
	color(5);
	SetConsoleCursorPosition(StdOut, Coord);
	SetConsoleTextAttribute(StdOut, FOREGROUND_RED);
	for (auto const i : arr) {
		for (auto const j : i) {
			if (!j) {
				std::cout << " ";
			}
			else {
				std::cout << "X";
			}
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(StdOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	std::cout << "\nESC = Exit" << std::endl;
}