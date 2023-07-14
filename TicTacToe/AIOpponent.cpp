#include <iostream>
#include "AIOpponent.h"
#include "Board.h"

int max(int a, int b)
{
	if (a > b) return a;
	else return b;
}

int min(int a, int b)
{
	if (a < b) return a;
	else return b;
}

int AIOpponent::minimax(int depth, bool isMax, int maxDepth)
{
	int evaluation = evaluate_board();
		
	if (evaluation == AI_FAVOR || evaluation == PLAYER_FAVOR) return evaluation;

	if (!Board::are_moves_left()) return NO_FAVOR;

	if (depth == maxDepth) return evaluation;

	if (isMax)
	{
		int bestEval = -100000;

		for (int x = 0; x < Board::BOARD_SIZE; x++)
		{
			for (int y = 0; y < Board::BOARD_SIZE; y++)
			{
				if (Board::get_square_at(x, y) == Board::EMPTY)
				{
					// Perform the move
					Board::set_square_at(x, y, Board::ENEMY);

					// Evaluate move recursively
					bestEval = max(bestEval, minimax(depth + 1, !isMax, maxDepth));

					// Undo the previous move
					Board::set_square_at(x, y, Board::EMPTY);
				}
			}
		}

		return bestEval;
	}
	else
	{
		int bestEval = 100000;

		for (int x = 0; x < Board::BOARD_SIZE; x++)
		{
			for (int y = 0; y < Board::BOARD_SIZE; y++)
			{
				if (Board::get_square_at(x, y) == Board::EMPTY)
				{
					// Perform the move
					Board::set_square_at(x, y, Board::PLAYER);

					// Evaluate move recursively
					bestEval = min(bestEval, minimax(depth + 1, !isMax, maxDepth));

					// Undo the previous move
					Board::set_square_at(x, y, Board::EMPTY);
				}
			}
		}

		return bestEval;
	}
}

int AIOpponent::evaluate_board()
{
	for (int x = 0; x < Board::BOARD_SIZE; x++)
	{
		int rowSum = 0;
		int colSum = 0;
		for (int y = 0; y < Board::BOARD_SIZE; y++)
		{
			rowSum += Board::get_value_from(Board::get_square_at(x, y));
			colSum += Board::get_value_from(Board::get_square_at(y, x));
		}

		// Player wins
		if (rowSum == Board::PLAYER_WIN || colSum == Board::PLAYER_WIN)
		{
			return PLAYER_FAVOR;
		}
		// AI wins
		else if (rowSum == Board::ENEMY_WIN || colSum == Board::ENEMY_WIN)
		{
			return AI_FAVOR;
		}
	}

	int diagSum1 = Board::get_value_from(Board::get_square_at(0, 0)) + Board::get_value_from(Board::get_square_at(1, 1)) + Board::get_value_from(Board::get_square_at(2, 2));
	int diagSum2 = Board::get_value_from(Board::get_square_at(2, 0)) + Board::get_value_from(Board::get_square_at(1, 1)) + Board::get_value_from(Board::get_square_at(0, 2));

	// Player wins
	if (diagSum1 == Board::PLAYER_WIN || diagSum2 == Board::PLAYER_WIN)
	{
		return PLAYER_FAVOR;
	}
	// AI wins
	else if (diagSum1 == Board::ENEMY_WIN || diagSum2 == Board::ENEMY_WIN)
	{
		return AI_FAVOR;
	}

	// no winner
	return NO_FAVOR;
}

Move AIOpponent::evaluate_best_move()
{
	int bestEval = -100000;
	Move bestMove = Move::Move(-1, -1);

	for (int x = 0; x < Board::BOARD_SIZE; x++)
	{
		for (int y = 0; y < Board::BOARD_SIZE; y++)
		{
			// If square is empty
			if (Board::get_square_at(x, y) == Board::EMPTY)
			{
				// Perform move
				Board::set_square_at(x, y, Board::ENEMY);

				// Evaluate move (RECURSIVE ORIGIN)
				int moveEval = minimax(0, false, get_depth_from_difficulty());

				// Undo previous move
				Board::set_square_at(x, y, Board::EMPTY);

				// If the move that was just evaluated is the greatest, set this square as the best move
				if (moveEval > bestEval)
				{
					bestEval = moveEval;
					bestMove.row = x;
					bestMove.col = y;
				}
			}

		}
	}

	return bestMove;
}

int AIOpponent::get_depth_from_difficulty()
{
	if (difficulty == IMPOSSIBLE)
		return IMPOSSIBLE_DEPTH;
	else if (difficulty == VERY_HARD)
		return VERY_HARD_DEPTH;
	else if (difficulty == HARD)
		return HARD_DEPTH;
	else if (difficulty == MEDIUM)
		return MEDIUM_DEPTH;
	else if (difficulty == EASY)
		return EASY_DEPTH;
	else
		return IMPOSSIBLE_DEPTH;
}