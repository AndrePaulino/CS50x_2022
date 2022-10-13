#include <cs50.h>
#include <math.h>
#include <stdio.h>

void validate_card(long card_number);

int main(void)
{
    long card_number;
    card_number = get_long("Number: ");
    validate_card(card_number);
}

void validate_card(long card_number)
{
    long og_card_number = card_number;
    int digit;
    int digit_sum = 0;

    int i = 0;
    while (card_number)
    {
        // Get single digit counting backwards
        digit = card_number % 10;

        // Using a counter, get every other one, from second to last and double
        // it
        if (i % 2 != 0)
        {
            digit *= 2;
        }

        // Sum the digits
        digit_sum += digit % 10;
        digit_sum += digit / 10;

        // Remove that last digit and count up
        card_number /= 10;
        i++;
    }
    // Check its validity first parameter
    if (digit_sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        // Store initial digits to test the card company
        long two_initial_digits = og_card_number / (pow(10, i - 2));
        long first_digit = og_card_number / (pow(10, i - 1));

        // Test against companies indicators
        if (i == 13 && first_digit == 4)
        {
            printf("VISA\n");
        }
        else if (i == 15)
        {
            if (two_initial_digits == 34 || two_initial_digits == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (i == 16)
        {
            if (first_digit == 4)
            {
                printf("VISA\n");
            }
            else if (two_initial_digits == 51 || two_initial_digits == 52 ||
                     two_initial_digits == 53 || two_initial_digits == 54 ||
                     two_initial_digits == 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
}
