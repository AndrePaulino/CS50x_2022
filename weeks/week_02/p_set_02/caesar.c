#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);
string caeser_cipher(string plaintext, int key);
void print_info(string ciphertext);

int main(int argc, string argv[])
{

    unsigned int key;
    // Check programs parameters
    // Check if theres's more the one parameter (program name is 0)
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // Test if the parameter is numeral and extract the key
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        key = strtol(argv[1], &argv[1], 10);
    }

    // Tests passed, continue to program
    // Get user plaintext
    string plaintext = get_string("plaintext:  ");

    string ciphertext = caeser_cipher(plaintext, key);
    print_info(ciphertext);
}

// Loops a string checking each char, return true if all are digits, false
// otherwise
bool only_digits(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

string caeser_cipher(string plaintext, int key)
{
    string ciphertext = plaintext;

    // For the lenght of the text
    for (int i = 0, n = strlen(ciphertext); i < n; i++)
    {
        // If its a letter
        if (isalpha(plaintext[i]))
        {
            // As a int
            int c = plaintext[i];
            // Substract the ASCII equivalent of 'a' or 'A' accordingly to zero
            // out the alphabet, add the key and take the remainder of 26, so
            // loops properly accounting for keys bigger than 26
            if (isupper(plaintext[i]))
            {
                c -= 65;
                c = (c + key) % 26;
                c += 65;
            }
            else if (islower(plaintext[i]))
            {
                c -= 97;
                c = (c + key) % 26;
                c += 97;
            }

            // Add it to the ciphertext and if its not a letter, will keep it
            ciphertext[i] = c;
        }
    }

    return ciphertext;
}

void print_info(string ciphertext)
{
    // Create char array with the right lenght
    int n = strlen(ciphertext);
    char output[n + 12];

    // Create the output string by concatenating
    strcat(output, "ciphertext: ");
    strcat(output, ciphertext);

    printf("%s\n", output);
}
