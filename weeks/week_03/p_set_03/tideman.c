#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_cycle(int loser, int winner);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Go through the array grabing values in twos
    for (int i = 0; i < candidate_count; i++)
    {
        // Creating a preferences pair, adding a voter with said preference
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int n = sizeof(preferences[0]) / sizeof(preferences[0][0]);
    pair_count = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            // When not pairing with itself
            if (i != j)
            {
                // If not a tie
                if (preferences[i][j] != preferences[j][i])
                {
                    // Add the proper values to the array and count up
                    pairs[pair_count].winner = preferences[i][j] > preferences[j][i] ? i : j;
                    pairs[pair_count].loser = preferences[i][j] < preferences[j][i] ? i : j;
                    pair_count++;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Inverse selection sort
    int pref_diff1;
    int pref_diff2;

    int max_diff_idx;
    for (int i = 0; i < pair_count; i++)
    {
        // Initialize current max index
        max_diff_idx = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            // Get the votes of the prefered in twos, and check for max
            pref_diff1 = preferences[pairs[j].winner][pairs[j].loser];
            pref_diff2 = preferences[pairs[max_diff_idx].winner][pairs[max_diff_idx].loser];
            if (pref_diff1 > pref_diff2)
            {
                max_diff_idx = j;
            }
        }
        // In case of new max, swap the position
        if (max_diff_idx != i)
        {
            pair tmp = pairs[i];
            pairs[i] = pairs[max_diff_idx];
            pairs[max_diff_idx] = tmp;
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

bool check_cycle(int loser, int winner)
{
    // When the same, there a cycle
    if (loser == winner)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // Check if current loser wins at any already locked
        if (locked[loser][i])
        {
            // If so, does it win against original winner? There is a cycle
            if (check_cycle(i, winner))
            {
                return true;
            }
        }
    }
    // If function not aborted until here, there is no cycle
    return false;
}
// Print the winner of the election
// The winner is the source of the locked graph. A source is classified by not having any loss
void print_winner(void)
{
    // We go through all candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Set that his not a loser
        bool is_loser = false;
        // Compare him with all others candidates
        for (int j = 0; j < candidate_count; j++)
        {
            // If lost, its not our source, break, next candidate
            if (locked[j][i])
            {
                is_loser = true;
                break;
            }
        }
        // Reached here, is_loser remains false. That is our source, we break
        if (!is_loser)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
}
