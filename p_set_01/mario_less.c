#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int pyramid_height;

    do
    {
        printf("Provide a pyramid height between 1 and 8. \n");
        pyramid_height = get_int("Pyramid height: ");
    }
    while (pyramid_height < 1 || pyramid_height > 8);

    for (int i = 1; i <= pyramid_height; i++)
    {
        for (int j = pyramid_height; j > 0; j--)
        {
            if (j <= i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
