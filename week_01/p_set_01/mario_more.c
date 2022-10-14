#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int pyramid_height;

    // Prompt user
    do
    {
        printf("Provide a pyramid height between 1 and 8. \n");
        pyramid_height = get_int("Pyramid height: ");
    }
    while (pyramid_height < 1 || pyramid_height > 8);

    // Count rows
    for (int i = 1; i <= pyramid_height; i++)
    {
        // Count columns reversed
        for (int j = pyramid_height; j > 0; j--)
        {
            // Check if its decreasing and draw
            if (j <= i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }

        // Draw spacer
        printf("  ");

        // Count collumns forward
        for (int j = 0; j < pyramid_height; j++)
        {
            // Check if its increasing to continue or draw
            if (j >= i)
            {
                continue;
            }
            printf("#");
        }
        printf("\n");
    }
}
