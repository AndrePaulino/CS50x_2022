#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// A implementation of the Coleman-Liau algorithm

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int coleman_liau_score(string text);
void print_info(int grade);

int main(void)
{
    string text = get_string("Text: ");

    int grade = coleman_liau_score(text);

    print_info(grade);
}

int coleman_liau_score(string text)
{
    float letters_total = count_letters(text);
    float words_total = count_words(text);
    float sentences_total = count_sentences(text);

    float l = (letters_total / words_total) * 100;
    float s = (sentences_total / words_total) * 100;

    float index = 0.0588 * l - 0.296 * s - 15.8;

    return round(index);
}

int count_letters(string text)
{
    int letters = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (ispunct(text[i]))
        {
            if (text[i] == '.' || text[i] == '!' || text[i] == '?')
            {
                sentences++;
            }
        }
    }
    return sentences;
}

void print_info(int grade)
{
    // Convert the grade to string
    char grade_str[3];
    sprintf(grade_str, "%d", grade);

    // Create the result string to be printed
    char result[15] = "Grade ";
    if (grade >= 1 && grade < 16)
    {
        strcat(result, grade_str);
    }
    else if (grade >= 16)
    {
        strcat(result, "16+");
    }
    else
    {
        strcpy(result, "Before Grade 1");
    }

    printf("%s\n", result);
}
