#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // check if only one argument was passed
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filenames
    char *imageName = argv[1];

    // open input file
    FILE *filePointer = fopen(imageName, "r");
    if (filePointer == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", imageName);
        return 2;
    }


    // read file by blocks of 512 bytes
    BYTE currBlock[512];
    int imgIndex = -1;
    bool imgFound = false;
    FILE *outptr[50];
    char outFileName[8];

    while (feof(filePointer) == 0)
    {
        if (feof(filePointer) != 0)
        {
            break;
        }

        fread(&currBlock, sizeof(BYTE), 512, filePointer);


        if (currBlock[0] == 0xff && currBlock[1] == 0xd8 && currBlock[2] == 0xff && ((currBlock[3] & 0xf0) == 0xe0))
        {
            // close any previous open output file
            if (imgIndex >= 0)
            {
                fclose(outptr[imgIndex]);
            }

            imgIndex++;

            // indicate that we found the first image
            imgFound = true;

            // give a name to the output file
            sprintf(outFileName, "%03d.jpg", imgIndex);

            // open the i-th output file
            outptr[imgIndex] = fopen(outFileName, "w");
            if (outptr[imgIndex] == NULL)
            {
                fclose(filePointer);
                fprintf(stderr, "Could not create %i-th output file: %s.\n", imgIndex, outFileName);
                return 3;
            }

            // write inside the i-th outut file
            fwrite(&currBlock, sizeof(currBlock), 1, outptr[imgIndex]);
        }
        else
        {
            if (imgFound)
            {
                fwrite(&currBlock, sizeof(currBlock), 1, outptr[imgIndex]);
            }
        }

    }


    fclose(filePointer);
    fclose(outptr[imgIndex - 1]);
    return 0;
}
