#pragma once

class Board
{
	public:
		Board() = default;

		static const int BOARD_SIZE = 3;
		static const int PLAYER_WIN = 3;
		static const int ENEMY_WIN = -PLAYER_WIN;
		static const char EMPTY = '_';
		static const char PLAYER = 'X';
		static const char ENEMY = 'O';
		
		static char get_square_at(int row, int col);
		static bool are_moves_left();
		static int get_value_from(char c);
		static int check_winner();
		static void set_square_at(int row, int col, char c);
		static void display_board();
		static void reset_board();

	private:
		static void display_row(int row);
		inline static char board[3][3] = {
			{EMPTY, EMPTY, EMPTY},
			{EMPTY, EMPTY, EMPTY},
			{EMPTY, EMPTY, EMPTY}
		};
};