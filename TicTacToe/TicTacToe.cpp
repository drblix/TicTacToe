// TODO:
// Add 2-player co-op mode
// 
// TicTacToe.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "Board.h"
#include "Move.h"
#include "AIOpponent.h"


void clear_console()
{
    system("cls");
}

Move response_to_move(std::string response)
{
    int row, col;

    // determining row
    switch (tolower(response[0]))
    {
        case 'a': row = 0; break;
        case 'b': row = 1; break;
        case 'c': row = 2; break;
        default: throw new std::invalid_argument("indistinguishable user response");
    }

    // determining column
    switch (response[1])
    {
        case '1': col = 0; break;
        case '2': col = 1; break;
        case '3': col = 2; break;
        default: throw new std::invalid_argument("indistinguishable user response");
    }

    return Move::Move(row, col);
}

bool is_response_valid(std::string response)
{
    if (response.length() > 2) return false;

    char first = tolower(response[0]);
    if (first != 'a' && first != 'b' && first != 'c') return false;

    char second = response[1];
    if (second != '1' && second != '2' && second != '3') return false;

    Move move = response_to_move(response);
    if (Board::get_square_at(move.row, move.col) != Board::EMPTY) return false;
    
    return true;
}


/// <summary>
/// Starts the opening title card sequence
/// </summary>
void title_card()
{
    using namespace std::this_thread;
    using namespace std::chrono;

    // wow this looks ugly after nullifying the escape characters

    std::string cards[] = { 
        " ______                      ______                           ______                   \n",
        "/\\__  _\\ __                 /\\__  _\\                         /\\__  _\\                  \n",
        "\\/_/\\ \\//\\_\\     ___        \\/_/\\ \\/     __       ___        \\/_/\\ \\/    ___      __   \n",
        "   \\ \\ \\\\/\\ \\   /'___\\  _______\\ \\ \\   /'__`\\    /'___\\  _______\\ \\ \\   / __`\\  /'__`\\ \n",
        "    \\ \\ \\\\ \\ \\ /\\ \\__/ /\\______\\\\ \\ \\ /\\ \\L\\.\\_ /\\ \\__/ /\\______\\\\ \\ \\ /\\ \\L\\ \\/\\  __/ \n",
        "     \\ \\_\\\\ \\_\\\\ \\____\\\\/______/ \\ \\_\\\\ \\__/.\\_\\\\ \\____\\\\/______/ \\ \\_\\\\ \\____/\\ \\____\\\n",
        "      \\/_/ \\/_/ \\/____/           \\/_/ \\/__/\\/_/ \\/____/           \\/_/ \\/___/  \\/____/\n",
        "\nby benjamin neuenschwander"
    };
    
    
    int startingFreq = 400;
    int change = 35;
    int arrayLength = sizeof(cards) / sizeof(cards[0]);

    // sizeof(cards) / sizeof(cards[0]) is array length
    for (int i = 0; i < arrayLength; i++)
    {
        
        if (i == arrayLength - 1)
        {
            std::cout << "\nby ";
            Beep(startingFreq, 250);

            sleep_for(milliseconds(200));
            std::cout << "benjamin ";
            Beep(startingFreq - 50, 150);

            sleep_for(milliseconds(100));
            std::cout << "neuenschwander";
            Beep(startingFreq, 500);
        }
        else
        {
            std::cout << cards[i];
            Beep(startingFreq, 200);
        }

        startingFreq += change;
        sleep_for(milliseconds(150));
    }

    sleep_for(seconds(2));

    clear_console();
}

int main()
{
    using namespace std::this_thread;
    using namespace std::chrono;

    // Intializing rand with seed
    srand(time(0));

    sleep_for(seconds(2));

    title_card();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    // Driver loop
    while (true)
    {
        Board::reset_board();

        // Difficulty select loop
        while (true)
        {
            clear_console();
            std::cout << "\n1 - Easy\n2 - Medium\n3 - Hard\n4 - Very Hard\n5 - Impossible :)\nEnter a number for the AI's difficulty: ";

            std::string choice = "";
            std::cin >> choice;

            if (choice == "1")
            {
                AIOpponent::difficulty = AIOpponent::EASY;
                break;
            }
            else if (choice == "2")
            {
                AIOpponent::difficulty = AIOpponent::MEDIUM;
                break;
            }
            else if (choice == "3")
            {
                AIOpponent::difficulty = AIOpponent::HARD;
                break;
            }
            else if (choice == "4")
            {
                AIOpponent::difficulty = AIOpponent::VERY_HARD;
                break;
            }
            else if (choice == "5")
            {
                AIOpponent::difficulty = AIOpponent::IMPOSSIBLE;
                break;
            }
        }

        // Primary game loop
        while (true)
        {
            clear_console();
            Board::display_board();

            if (Board::check_winner() == -1)
            {
                Beep(400, 450);
                Beep(300, 450);
                Beep(200, 450);
                sleep_for(seconds(3));
                break;
            }

            std::string userResponse = "N";

            do
            {
                if (userResponse != "N")
                {
                    clear_console();
                    Board::display_board();
                
                    // Displays text as red
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "\nunidentified response or square already filled\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }

                std::cout << "\nYour turn!\nChoose a square to place an '" << Board::PLAYER << "' (Example: a2): ";
                std::cin >> userResponse;
            } while (!is_response_valid(userResponse));

            // Converting player's response into a usable move datatype
            Move player1Move = response_to_move(userResponse);
            // Performing the player's move
            Board::set_square_at(player1Move.row, player1Move.col, Board::PLAYER);

            Beep(500, 400);

            clear_console();

            // Displaying the player's move and checking if there's a winner
            Board::display_board();

            if (Board::check_winner() == 1)
            {
                Beep(200, 450);
                Beep(300, 450);
                Beep(400, 450);
                sleep_for(seconds(3));
                break;
            }

            // AI's turn to move now
        
            // Illusion that computer is "thinking"
            std::cout << "\nComputer's turn!\nThinking...";

            // Getting optimal move
            Move optimalMove = AIOpponent::evaluate_best_move();

            // Storing previous difficulty
            std::string prevDifficulty = AIOpponent::difficulty;

            // roll 1 - 10
            int roll = rand() % 10 + 1;
            bool metRoll = false;

            // Pseudoway of the AI making "mistakes" during gameplay
            // Performing a search with a lower depth than that of its own difficulty
            if (AIOpponent::difficulty != AIOpponent::EASY && AIOpponent::difficulty != AIOpponent::IMPOSSIBLE)
            {
                // 10% chance to make bad move on very hard
                if (AIOpponent::difficulty == AIOpponent::VERY_HARD)
                {
                    AIOpponent::difficulty = AIOpponent::HARD;
                    metRoll = roll == 10;
                }
                // 20% chance to make bad move on hard
                else if (AIOpponent::difficulty == AIOpponent::HARD)
                {
                    AIOpponent::difficulty = AIOpponent::MEDIUM;
                    metRoll = roll >= 9;
                }
                // 30% chance to make bad move on medium
                else if (AIOpponent::difficulty == AIOpponent::MEDIUM)
                {
                    AIOpponent::difficulty = AIOpponent::EASY;
                    metRoll = roll >= 8;
                }
            }

            // Was the difficulty decrease roll met?
            if (metRoll)
                optimalMove = AIOpponent::evaluate_best_move();

            AIOpponent::difficulty = prevDifficulty;
        
            // Waiting for random amount of time
            int randomTime = rand() % 1001 + 2000;
            sleep_for(milliseconds(randomTime));

            // Performing previously calculated move

            if (optimalMove.row != -1 && optimalMove.col != -1)
                Board::set_square_at(optimalMove.row, optimalMove.col, Board::ENEMY);
            else if (!Board::are_moves_left())
            {
                clear_console();
                Board::display_board();
                std::cout << "\nTie! No one wins\n";
                Beep(300, 800);
                sleep_for(seconds(3));
                break;
            }

            Beep(300, 400);
        }

        // Prompt for if the user wants to play again
        clear_console();

        std::cout << "Play again? (y/n): ";
        std::string response;
        std::cin >> response;

        if (response == "N" || response == "n")
        {
            clear_console();
            std::cout << "Goodbye!";
            
            Beep(400, 450);
            Beep(300, 200);
            Beep(400, 450);
            Beep(200, 600);

            sleep_for(seconds(2));
            break;
        }
    }

    return 0;
}
