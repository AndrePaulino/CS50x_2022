#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool validate_key(string key);
string substitution_cipher(string plaintext, string key);
void print_info(string ciphertext);

int main(int argc, string argv[])
{

    // Check programs parameters
    // If theres's more the one parameter (program name is 0)
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Test if the parameter is a valid key
    string key;
    if (!validate_key(argv[1]))
    {
        return 1;
    }
    else
    {
        key = argv[1];
    }

    // Tests passed, continue to program
    // Get user plaintext
    string plaintext = get_string("plaintext:  ");

    string ciphertext = substitution_cipher(plaintext, key);
    print_info(ciphertext);
}

// Loops a string checking each char, return true if all are digits, false
// otherwise
bool validate_key(string key)
{
    // Get the length of string, if not 26, invalid
    int n = strlen(key);
    if (n != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }
    // Create a array to keep each char
    char checker[26];
    // Loop through the string
    for (int i = 0; i < n; i++)
    {
        // Loop through the array and if the current char already exist in it,
        // that char is repeated
        for (int j = 0; j < i; j++)
        {
            if (tolower(checker[j]) == tolower(key[i]))
            {
                printf("Key must not contain repeated characters.\n");
                return false;
            }
        }
        // Check if the char is a letter
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return false;
        }
        // Add the char to the checker array
        checker[i] = key[i];
    }
    // Else is a valid key
    return true;
}

string substitution_cipher(string plaintext, string key)
{
    // string alphabet = "abcdefghijklmnopqrstuvwxyz";
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                       'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    string ciphertext = plaintext;

    // For the length of the text
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];

        if (isalpha(c))
        {
            int key_idx;
            c = tolower(c);
            // For the length of the alphabet
            for (int j = 0; j < 26; j++)
            {
                // Get the position of current char on the alphabet
                if (c == alphabet[j])
                {
                    key_idx = j;
                }
            }
            // Get that position on the key
            c = key[key_idx];
        }
        // Apply the proper case
        c = islower(plaintext[i]) ? tolower(c) : toupper(c);
        // Put it on the ciphertext
        ciphertext[i] = c;
    }

    return ciphertext;
}

void print_info(string ciphertext)
{
    // Create char array with the right length
    int n = strlen(ciphertext);
    char output[n + 12];

    // Create the output string by concatenating
    strcat(output, "ciphertext: ");
    strcat(output, ciphertext);

    printf("%s\n", output);
}
