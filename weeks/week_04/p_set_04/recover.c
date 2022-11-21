#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;
typedef uint8_t BYTE;

void recoverJpgs(FILE *input);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    recoverJpgs(input);

    fclose(input);
    return 0;
}

void recoverJpgs(FILE *input)
{
    BYTE buffer[BLOCK_SIZE];
    FILE *output = NULL;

    int jpgCount = 0;
    // NOTE: "./images/" to file name for organization sake.
    // For check50, jpgname size should be 8 and remove "./images" from name on line 53
    char jpgName[17];

    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, input))
    {
        // If it's the begginning of a JPEG.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Begginning of one means the end of another, and if one exists, close it.
            if (jpgCount)
            {
                fclose(output);
            }

            // Create the new output file name and open it for writing.
            snprintf(jpgName, sizeof(jpgName), "./images/%03i.jpg", jpgCount);
            output = fopen(jpgName, "w");
            // Count up a new file.
            jpgCount++;
        }

        // If a file exists, write the block into the output.
        if (jpgCount)
        {
            fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }
    fclose(output);
}
