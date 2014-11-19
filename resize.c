/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int num_size = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // Check good value of resize:
    if ((num_size < 1) || (num_size > 100))
    {
        printf("Need positive scaling value 1 to 100\n");
        return 1;
    }
    
    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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
    
    // Save what we need
    LONG old_height = bi.biHeight;
    LONG old_width = bi.biWidth;
    int old_padding = (4 - (old_width * sizeof(RGBTRIPLE)) % 4) % 4;
    int old_size_image = bi.biSizeImage;
    
    // update size parameters
    bi.biWidth = old_width*num_size;
    bi.biHeight = old_height*num_size;
    int abs_height = abs(bi.biHeight);
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    //bi.biSizeImage = bi.biWidth*(abs(bi.biHeight))*(sizeof(RGBTRIPLE)+padding);
    bi.biSizeImage = bi.biWidth*(abs(bi.biHeight))*(sizeof(RGBTRIPLE)) + padding*abs_height;
    
    bf.bfSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) + bi.biSizeImage;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
   

    int lines = 1;
    long spot = 0;

    // iterate over infile's scanlines
    for (int i = 0;  i < abs_height; i++)
    {
       spot = ftell(inptr);

        // iterate over pixels in scanline
        for (int j = 0; j < old_width; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            for (int ix = 0; ix < num_size; ix++)
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, old_padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
        
        if (lines == num_size)
        {
            lines = 1;
        }
        else
        {   
            fseek(inptr, spot, SEEK_SET);
            lines++;
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
