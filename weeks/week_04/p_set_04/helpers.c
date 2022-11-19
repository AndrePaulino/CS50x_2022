#include "helpers.h"
#include <math.h>
#include <stdio.h>

// TODO: For automated tests
float clamp(float num, float min, float max);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            // get the average of the pixels rgb values and apply them back.
            buffer = image[i][j];
            int grayAvg = round((buffer.rgbtBlue + buffer.rgbtGreen + buffer.rgbtRed) / 3.0);

            buffer.rgbtBlue = grayAvg;
            buffer.rgbtGreen = grayAvg;
            buffer.rgbtRed = grayAvg;

            image[i][j] = buffer;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            // Appy the sepia equation to the individual rgb pixel values.
            buffer = image[i][j];

            int redPx =
                round(clamp(0.393 * buffer.rgbtRed + 0.769 * buffer.rgbtGreen + 0.189 * buffer.rgbtBlue, 0, 255));
            int greenPx =
                round(clamp(0.349 * buffer.rgbtRed + 0.686 * buffer.rgbtGreen + 0.168 * buffer.rgbtBlue, 0, 255));
            int bluePx =
                round(clamp(0.272 * buffer.rgbtRed + 0.534 * buffer.rgbtGreen + 0.131 * buffer.rgbtBlue, 0, 255));

            image[i][j].rgbtRed = redPx;
            image[i][j].rgbtGreen = greenPx;
            image[i][j].rgbtBlue = bluePx;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For all the height and half the width invert the pixel positions.
    // Going from the start and end of width.
    RGBTRIPLE buffer;
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width / 2, countdown = width - 1; j < m; j++, countdown--)
        {
            buffer = image[i][j];
            image[i][j] = image[i][countdown];
            image[i][countdown] = buffer;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("BLUR\n");
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each RGBTRIPLE go throught its adjacents.
            // Grab the original rgb values and appy its average.
            float redPx = 0, greenPx = 0, bluePx = 0;
            int adjCount = 0;

            for (int dx = (i > 0 ? -1 : 0); dx <= (i < height - 1 ? 1 : 0); dx++)
            {
                for (int dy = (j > 0 ? -1 : 0); dy <= (j < width - 1 ? 1 : 0); dy++)
                {
                    redPx += buffer[i + dx][j + dy].rgbtRed;
                    greenPx += buffer[i + dx][j + dy].rgbtGreen;
                    bluePx += buffer[i + dx][j + dy].rgbtBlue;
                    adjCount++;
                }
            }

            image[i][j].rgbtRed = round(redPx / adjCount);
            image[i][j].rgbtGreen = round(greenPx / adjCount);
            image[i][j].rgbtBlue = round(bluePx / adjCount);
        }
    }
}

// Clamp float
float clamp(float num, float min, float max)
{
    float clamped = num < min ? min : num;
    return clamped > max ? max : clamped;
}
