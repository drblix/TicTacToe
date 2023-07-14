#pragma once
#include "Move.h"

class AIOpponent
{
	public:
		inline static const std::string IMPOSSIBLE = "impossible";
		inline static const std::string VERY_HARD = "very hard";
		inline static const std::string HARD = "hard";
		inline static const std::string MEDIUM = "medium";
		inline static const std::string EASY = "easy";

		inline static std::string difficulty = MEDIUM;
		static Move evaluate_best_move();
	private:
		static const int PLAYER_FAVOR = -1;
		static const int AI_FAVOR = 1;
		static const int NO_FAVOR = 0;

		// Difficulty settings
		static const int EASY_DEPTH = 1;
		static const int MEDIUM_DEPTH = 2;
		static const int HARD_DEPTH = 4;
		static const int VERY_HARD_DEPTH = 6;
		static const int IMPOSSIBLE_DEPTH = -1;

		static int minimax(int depth, bool isMax, int maxDepth = -1);
		static int evaluate_board();
		static int get_depth_from_difficulty();
};