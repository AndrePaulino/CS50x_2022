#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void print_winner(int score1, int score2);

// Points assigned to each letter of the alphabet
int POINTS[] =
{
    1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
    1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10,
};

char ALPHABET[] =
{
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    print_winner(score1, score2);
}

int compute_score(string word)
{
    int total_points = 0;
    // Loop through the word
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // If it's a letter
        if (isalpha(word[i]))
        {
            int char_idx;
            // Get the index of the letter on the alphabet
            for (int j = 0, m = sizeof(ALPHABET); j < m; j++)
            {
                if (tolower(word[i]) == ALPHABET[j])
                {
                    char_idx = j;
                }
            }
            // Apply the corresponding pontuation
            total_points += POINTS[char_idx];
        }
    }

    return total_points;
}

void print_winner(int score1, int score2)
{
    if (score1 != score2)
    {
        int winner = score1 > score2 ? 1 : 2;
        printf("%i\n", winner);
        printf("Player %i wins!\n", winner);
    }
    else
    {
        printf("Tie!\n");
    }
}
