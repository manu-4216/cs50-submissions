// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    char n = atoi(argv[1]);
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Determine the old padding for scanlines
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    // Update BITMAPINFOHEADER: biWidth, biHeight, then new padding, then biSizeImage
    bi.biWidth *= n;
    bi.biHeight *= n;

    // Determine new padding for scanlines
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight);


    // Update BITMAPFILEHEADER: bfSize
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // Allocate dynamically memory to hold an array of triples, of dynamic length
    RGBTRIPLE * newRow = malloc(bi.biWidth * sizeof(RGBTRIPLE));

    // iterate over infile's scanlines
    for (int i = 0, oldBiHeight = abs(bi.biHeight / n); i < oldBiHeight; i++)
    {
        // Reset the previous array (previous row) - not needed, just override it.

        // iterate over pixels in scanline
        for (int j = 0, oldBiWidth = bi.biWidth / n; j < oldBiWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // Write the triple n times to newRow
            for (int k = 0; k < n; k++)
            {
                newRow[j * n + k] = triple;
            }
        }

        // Write the newRow+padding n times
        for (int k = 0; k < n; k++)
        {
            // write the newRow to outfile, one item (triple) at a time
            for (int l = 0; l < bi.biWidth; l++)
            {
                fwrite(&newRow[l], sizeof(RGBTRIPLE), 1, outptr);
            }

            // Write the padding at the end of the new row
            for (int m = 0; m < newPadding; m++)
            {
                fputc(0x00, outptr);
            }
        }

        // skip over oldPadding, if any
        fseek(inptr, oldPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // free memory
    free(newRow);

    // success
    return 0;
}
