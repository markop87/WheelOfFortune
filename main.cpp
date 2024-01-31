/*
    A game based on the popular TV show Wheel of Fortune.
*/

#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <random>
#include <sstream>
#include <bits/stdc++.h>
#include "windows.h"

//struct for puzzles which players will be guessing
struct Puzzle {
    std::string category;
    std::string puzzle;

    //Puzzle struct constructor
    Puzzle (std::string c, std::string p)
    {
        category = c;
        puzzle = p;
    }
};

//vector to hold list of puzzles
std::vector <Puzzle> puzzles {};

//loading puzzles from csv file
void loadPuzzles(std::vector <Puzzle> &puzzles) {

    std::ifstream inputFile;
    inputFile.open("puzzles.csv");

    std::string line = "";

    while(getline(inputFile, line)) {

        std::string category;
        std::string puzzle;

        std::stringstream inputString(line);

        getline(inputString, category, ';');
        getline(inputString, puzzle, ';');

        Puzzle newPuzzle(category, puzzle);
        puzzles.push_back(newPuzzle);

        line = "";
    }

    inputFile.close();
}

//vector with values on wheel
std::vector <int> wheel {
2500, 300, 600, 700, 600, 650, 500,
700, -1, 600, 550, 500, 600, -1, 650,
250, 700, 0, 800, 500, 650, 500, 900, 400 };

//function to check whether the puzzle and the guessed letters are the same
//when player didn't solve the puzzle, but guessed all the letters
bool areSame(int arr[], int n)
{
  for (int i = 0; i < n; i++)
    if (arr[i] != 1)
      return 0;
  return 1;
}

//function to check if input is a vowel (Y in English alphabet is consonant)
bool checkVowel(char x)
{
    if(x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U')
    {
        return true;
    } else return false;
}

int main()
{
    loadPuzzles(puzzles); //loading puzzles from file to vector
    std::cout << "Wheel of Fortune!\n\n"; //title of a game

    int current_round[3] {0, 0, 0}; // array to save points in current round
    int total_points[3] {0, 0, 0}; // array to save points from every round

    //player variable indicates which player's turn it is
    //players variable for number of players
    int player {1}, players {0};

    //determining how many players will play
    while(players < 1 || players > 3)
    {
        std::cout << "How many players? (1-3): ";
        std::cin >> players;
        std::cin.clear();
        std::cin.ignore(256,'\n');
    }

    //generating random number
    static std::mt19937 gen(time(nullptr));; // seed the generator
    std::uniform_int_distribution<> distr(0, puzzles.size()-1); //range

    // Loop for 6 rounds, 5 rounds are for all players
    // and 6th rounds is a final for player with most points
    for(int r = 1; r < 7; r++)
    {
        if(r < 6)
        {
            int draw = distr(gen); // drawing a puzzle

            std::string puzzle = puzzles[draw].puzzle; // drawing a puzzle
            int chars = puzzle.size(); // checking length of final puzzle
            std::string category = puzzles[draw].category; // assigning a randomly selected category
            bool solved {false}; // checking if the puzzle has been solved
            int checkers[chars] = {}; // creating array to check the occurrence of letters

            while(!solved)
            {
                system("CLS"); // clearing screen

                std::cout << "== Round " << r << " ==\n\n"; // displaying round

                std::cout << "CATEGORY: " << category << "\n\n"; // displaying category

                bool good_answer{false}; // a variable to mark whether the player's answer was good

                // displaying the puzzle
                // if there is a space or inter character, they will be
                // marked as guessed and will be displayed
                // Unguessed letters will be marked with underscore
                for(int i = 0; i < chars; i++)
                {
                    if(puzzle[i] == ' ')
                    {
                        std::cout << "    ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '-')
                    {
                        std::cout << "- ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '\'')
                    {
                        std::cout << "' ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '&')
                    {
                        std::cout << "& ";
                        checkers[i] = 1;
                    }
                    else if(checkers[i] == 1)
                    {
                        std::cout << puzzle[i] << " ";
                    }
                    else std::cout << "_ ";
                }

                std::cout << "\n\n\n" << std::string(60, '=') << "\n";

                char letter {}; // variable for the given letter
                std::string solving {}; // variable for the given puzzle
                int spin; // variable for spin of wheel

                // displaying players points
                for(int p = 0; p < players; p++)
                {
                    // a marker showing which player's turn it is
                    if(p+1 == player)
                    {
                        std::cout << "\n>>> ";
                    }
                    else std::cout << "\n    ";

                    std::cout << "Player " << p+1 << ": " << current_round[p] << " (" << total_points[p] << ")\n";
                }

                int choice {0}; // variable to save player's choice

                // two menu variants, if the player does not have enough points, he will not have the option to buy a vowel
                if(current_round[player-1] >= 250)
                {
                    std::cout << "\n\nWhat do you want to do?\n1. Spin the wheel\n2. Buy a vowel (-250)\n3. Solve the puzzle\n\n";

                    while(choice != 1 && choice != 2 && choice != 3)
                    {
                        std::cout << "Your choice: ";
                        std::cin >> choice;
                        std::cin.clear();
                        std::cin.ignore(256,'\n');
                    }
                }
                else
                {
                    std::cout << "\n\nWhat do you want to do?\n1. Spin the wheel\n3. Solve the puzzle\n\n";

                    while(choice != 1 && choice != 3)
                    {
                        std::cout << "Your choice: ";
                        std::cin >> choice;
                        std::cin.clear();
                        std::cin.ignore(256,'\n');
                    }
                }

                // if player spin the wheel to guess a consonant
                if(choice == 1)
                {
                    static std::mt19937 gen(time(nullptr));; // seed the generator
                    std::uniform_int_distribution<> distr(0, 23); //range

                    spin = distr(gen); // drawing random number for wheel vector

                    // if wheel shows 0, the player loses his turn
                    if(wheel[spin] == 0)
                    {
                        std::cout << "On wheel: Lose a turn!\n";
                        Sleep(1000);
                        player++; // changing player
                        if(player > players) { player = 1; } // if it was the last player, the counter returns to the beginning
                    }

                    // if wheel shows -1, the player loses the points earned in this round and loses his turn
                    else if(wheel[spin] == -1)
                    {
                        current_round[player-1] = 0;
                        std::cout << "On wheel: Bankrupt!\n";
                        Sleep(1000);
                        player++;
                        if(player > players) { player = 1; }
                    }
                    // if wheels shows value higher than 0, the player can say a consonant
                    else
                    {
                        std::cout << "On wheel: " << wheel[spin] << "\n";

                        std::cout << "\nEnter the consonant: ";
                        std::cin >> std::setw(1) >> letter; // saving only first letter from player's input
                    }

                }

                // if player wants to buy vowel
                else if(choice == 2)
                {
                    current_round[player-1] -= 250; // deducting points for buying a vowel
                    std::cout << "\nEnter the vowel: ";
                    std::cin >> std::setw(1) >> letter;
                }

                // if player wants to solve the puzzle
                else if(choice == 3)
                {
                    std::cin.clear();
                    std::cout << "\nSolve the puzzle: ";
                    getline(std::cin, solving);
                    transform(solving.begin(), solving.end(), solving.begin(), ::toupper);

                }

                // checking if player guessed letter
                if(choice < 3)
                {
                    letter = toupper(letter);

                    for(int l = 0; l < chars; l++)
                    {
                        if(letter == puzzle[l] && checkers[l] != 1 && ((choice == 1 && !checkVowel(letter)) || (choice == 2 && checkVowel(letter))))
                        {
                            checkers[l] = 1; // indicating that the letter has been guessed
                            good_answer = true; // indicating that the player was right

                            //adding points from wheel to player's current round points
                            if(choice == 1)
                            {
                                current_round[player-1] += wheel[spin];
                            }
                        }
                    }
                }

                // if puzzle was guessed
                if(areSame(checkers, chars) || solving == puzzle)
                {
                    std::cout << "\nYou've won this round!";
                    Sleep(750);
                    solved = true; // indicating that the puzzle was guessed

                    // adding points to winner of a round and nulling score for current round
                    total_points[player-1] += current_round[player-1];
                    current_round[0] = 0;
                    current_round[1] = 0;
                    current_round[2] = 0;
                    good_answer = true;
                }

                // if letter was guessed
                if(good_answer && !solved)
                {
                    std::cout << "\nYou're right!";
                    Sleep(750);
                }

                // if letter wasn't guessed
                else if(!good_answer && !solved && wheel[spin] > 0)
                {
                    std::cout << "\nWrong.";
                    Sleep(750);

                    player++;
                    if(player > players) { player = 1; }
                }
            }

        }
        else
        {
            system("CLS"); // cleaning screen

            // displaying results after 5 rounds
            // and checking which player has most points
            std::cout << "== RESULTS AFTER 5. ROUNDS ==\n";

            int winner {0};
            int highScore {0};

            for(int p = 1; p < 4; p++)
            {
                std::cout << "Player " << p << ": " << total_points[p-1] << "\n";

                if (total_points[p-1] > highScore)
                {
                    winner = p;
                    highScore = total_points[p-1];
                }
            }
            // finalist announcement
            std::cout << "\nPlayer " << winner << " will play in the final!\n\n";

            system("pause"); // waiting for player to press button to continue

            int draw;
            std::string category {};
            std::string puzzle {};

            do
            {
                draw = distr(gen); // drawing a puzzle
                category = puzzles[draw].category; // assigning a randomly selected category
                puzzle = puzzles[draw].puzzle; // assigning a randomly selected puzzle
            } while(puzzles[draw].puzzle.size() < 21); // for final drawing a longer puzzle

            int chars = puzzle.size(); // checking length of final puzzle
            int checkers[chars] = {}; // creating array to check the occurrence of letters

            // array with letter for final puzzle player automatically
            // gets 6 letter and can add four letters of their own
            char final_letters[10] {'R', 'S', 'T', 'L', 'N', 'E'};

            // The program will perform two loops. In the first one, player will
            // see a puzzle with revealed letters that he received automatically
            // and will be able to choose 3 consonants and one vowel.
            // In the second loop, he will see a puzzle with exposed letters
            // that he guessed himself and will have to guess the puzzle.
            for(int f = 0; f < 2; f++)
            {
                system("CLS"); // clear screen

                std::cout << "== FINAL == \n\n"; // title
                std::cout << "Category: " << category << "\n\n"; // displaying cattegory

                //information about automatically received letters
                if(f == 0)
                {
                    std::cout << "5 consonants and 1 vowel for you:\nR S T L N and E\n\n";
                }

                //checking letters, like in previous rounds
                for(int f = 0; f < 10; f++)
                {
                    for(int l = 0; l < chars; l++)
                    {
                        if(final_letters[f] == puzzle[l] && checkers[l] != 1)
                        {
                            checkers[l] = 1;
                            current_round[player-1] += 1;
                        }
                    }
                }

                //displaying puzzle, like in previous rounds
                for(int i = 0; i < chars; i++)
                {
                    if(puzzle[i] == ' ')
                    {
                        std::cout << "    ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '-')
                    {
                        std::cout << "- ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '\'')
                    {
                        std::cout << "' ";
                        checkers[i] = 1;
                    }
                    else if(puzzle[i] == '&')
                    {
                        std::cout << "& ";
                        checkers[i] = 1;
                    }
                    else if(checkers[i] == 1)
                    {
                        std::cout << puzzle[i] << " ";
                    }
                    else std::cout << "_ ";
                }

                if(f == 0)
                {
                    std::cout << "\n\nNow choose your 3 consonants and 1 vowel.\n\n";

                    char final_letter {};

                    for(int c = 1; c < 5; c++)
                    {
                        if(c < 4)
                        {
                            //Player has to give 3 consonants
                            //if player give a vowel, the question will be repeated

                            bool consonant {true};

                            while(consonant)
                            {
                                std::cout << "Your " << c << ". consonant: ";
                                std::cin >> final_letter;
                                final_letter = toupper(final_letter);

                                consonant = checkVowel(final_letter); //checking if given letter is vowel
                            }
                            //assigning given letter to array
                            final_letters[c + 5] = final_letter;
                        }
                        else
                        {
                            //Player has to give 1 vowel
                            //if player give a consonant, the question will be repeated

                            bool vowel {false};

                            while(!vowel)
                            {
                                std::cout << "Your vowel: ";
                                std::cin >> final_letter;
                                final_letter = toupper(final_letter);

                                vowel = checkVowel(final_letter); //checking if given letter is vowel
                            }

                            //assigning given letter to array
                            final_letters[c + 5] = final_letter;
                        }
                    }
                }

                //guessing the final puzzle
                if(f == 1)
                {
                    std::string final_guess {}; //variable for player's guess

                    std::cout << "\n\nIt's time to guess a final puzzle!\n\n";

                    std::cout << "Your guess: ";
                    std::cin.ignore();
                    getline(std::cin, final_guess);
                    transform(final_guess.begin(), final_guess.end(), final_guess.begin(), ::toupper);

                    if(final_guess == puzzle)
                    {
                        std::cout << "\nYou won! Congratulations!\n"; // if the player solved the puzzle
                    }
                    // if the player has not solved the puzzle, the correct answer will be given
                    else std::cout << "\nOh, we're sorry! You lost. The puzzle was: " << puzzle << "\n\n";
                }
            }
        }
    }

    return 0;
}
