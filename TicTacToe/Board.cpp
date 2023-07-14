#include <iostream>
#include <Windows.h>
#include "Board.h"

bool Board::are_moves_left()
{
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (get_square_at(x, y) == '_') return true;

	return false;
}

char Board::get_square_at(int row, int col)
{
	return board[row][col];
}

/// <summary>
/// Gets the value of the provided character
/// </summary>
/// <param name="c">Must be EMPTY, PLAYER, or ENEMY constant from the board class</param>
/// <returns>0 if empty; 1 if player; -1 if enemy</returns>
int Board::get_value_from(char c)
{
	switch (c)
	{
		case EMPTY: return 0;
		case PLAYER: return 1;
		case ENEMY: return -1;
		default: throw new std::invalid_argument("invalid character provided");
	}
}

void Board::set_square_at(int row, int col, char c)
{
	board[row][col] = c;
}

int Board::check_winner()
{
	// Checking each row and column for a three-in-a-row
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		int rowSum = 0;
		int colSum = 0;
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			rowSum += get_value_from(get_square_at(x, y));
			colSum += get_value_from(get_square_at(y, x));
		}

		if (rowSum == PLAYER_WIN || colSum == PLAYER_WIN)
		{
			std::cout << "\nPlayer wins!\n";
			return 1;
		}
		else if (rowSum == ENEMY_WIN || colSum == ENEMY_WIN)
		{
			std::cout << "\nComputer wins!\n";
			return -1;
		}
	}

	// Checking all possible diagonals for three-in-a-rows
	int diagSum1 = get_value_from(get_square_at(0, 0)) + get_value_from(get_square_at(1, 1)) + get_value_from(get_square_at(2, 2));
	int diagSum2 = get_value_from(get_square_at(2, 0)) + get_value_from(get_square_at(1, 1)) + get_value_from(get_square_at(0, 2));

	if (diagSum1 == PLAYER_WIN || diagSum2 == PLAYER_WIN)
	{
		std::cout << "\nPlayer wins!\n";
		return 1;
	}
	else if (diagSum1 == ENEMY_WIN || diagSum2 == ENEMY_WIN)
	{
		std::cout << "\nComputer wins!\n";
		return -1;
	}

	return 0;
}

void Board::display_board()
{
	/* Template for output
	   0   1   2
	A  _ | _ | _
	  -----------
	B  _ | _ | _
	  -----------
	C  _ | _ | _
	*/
	std::cout << "   1   2   3\n";
	std::cout << "A  ";
	display_row(0);
	std::cout << "  -----------\n";
	std::cout << "B  ";
	display_row(1);
	std::cout << "  -----------\n";
	std::cout << "C  ";
	display_row(2);
}

void Board::display_row(int row)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int col = 0; col < BOARD_SIZE; col++)
	{
		char c = get_square_at(row, col);
		
		if (c == PLAYER)
			SetConsoleTextAttribute(hConsole, 12);
		else if (c == ENEMY)
			SetConsoleTextAttribute(hConsole, 9);
		else
			SetConsoleTextAttribute(hConsole, 15);

		std::cout << get_square_at(row, col);

		SetConsoleTextAttribute(hConsole, 15);
		if (col != 2)
			std::cout << " | ";
		else
			std::cout << "\n";
	}
}

void Board::reset_board()
{
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			set_square_at(x, y, EMPTY);
}
