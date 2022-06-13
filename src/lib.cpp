#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "lib.hpp"
#include "lib/algorithms/naive.cpp"

Guess::Guess(std::string word, std::array<Correctness, 5> mask) : word( word ), mask( mask ) {};

/**
* Compare a guess and an answer and update a correctness array with the status of each character.
*
* @param answer a std::string that represents the correct answer.
* @param guess a std::string that represents the guess.
* @returns an std::array<Correctness, 5> that has the current status of each char in the guess.
*/
std::array<Correctness, 5> check(std::string answer, std::string guess)
{
    // Ensure that the answer and guess are the correct size
    assert(answer.size() == 5);
    assert(guess.size() == 5);

    std::array<Correctness, 5> c;

    // Mark characters as to wether they are correct or not
    for (int i = 0; i < 5; ++i)
    {
        char a = answer[i];
        char g = guess[i];
        if (a == g) // Mark the character green (Correct)
        {
            c[i] = Correctness::Correct;
        }
        else if (guess.find(a) != std::string::npos) // Mark the character yellow (Misplaced)
        {
            c[i] = Correctness::Misplaced;
        }
        else // Mark the character grey (Wrong)
        {
            c[i] = Correctness::Wrong;
        }
    }

    return c;
}

size_t play(std::string answer, Guesser* G)
{
    std::vector<Guess> history;

    for (size_t i = 1; i < 32; ++i) // Wordle only allows 6 guesses. But for stats purposes we allow more.
    {
        std::string guess = G->guess(history);
        if (guess == answer)
        {
            return i;
        }

        std::array<Correctness, 5> correctness = check(answer, guess);
        history.push_back(Guess(guess, correctness));
    }

    return -1;
}

void make_hash_map_from_dictionary(std::unordered_map<std::string, int>& map, std::string filepath)
{
    std::ifstream file;
    file.open(filepath);
    if (!file.is_open())
    {
        std::cerr << "Could not open file located at " << filepath << " exiting program" << std::endl;
    }

    std::string word;
    int frequency;
    while (file >> word >> frequency)
    {
        map.insert(std::make_pair(word, frequency));
    }
}

bool cmp(std::pair<std::string, int>& a, std::pair<std::string, int>& b)
{
    return a.second < b.second;
}

void sort_map_by_frequency(std::unordered_map<std::string, int>& map)
{
    std::vector<std::pair<std::string, int> > a;
    std::unordered_map<std::string, int> m;

    for (auto& it : map)
    {
        a.push_back(it);
    }

    sort(a.begin(), a.end(), cmp);

    for (auto& it : a)
    {
        m.insert(it);
    }

    map = m;
}

#endif
