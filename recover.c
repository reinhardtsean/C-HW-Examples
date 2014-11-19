/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* inptr = fopen("card.raw", "r");
    FILE* outptr;
    char buffer[512];
    int bytes_read = 0;
    
    char header1[4] = {0xff,0xd8,0xff,0xe0};
    char header2[4] = {0xff,0xd8,0xff,0xe1};
    
    if (inptr == NULL)
    {
        printf("Could not open file");
        return 2;
    }
    
 
    int count = 0;
    int keep_reading = 1;
    int reading_jpg = 0;
    char filename[6];
    while (keep_reading == 1)
    {
        
        if (!(feof(inptr)))
            fread(&buffer, 512, 1, inptr);
        else
            keep_reading = 0;
        if (feof(inptr))
            keep_reading = 0;    
            
        
        if((buffer[0]==header1[0])&&(buffer[1]==header1[1])&&(buffer[2]==header1[2])
            &&((buffer[3]==header1[3])||(buffer[3]==header2[3])))
        {
            if (reading_jpg == 1)
            {
                fclose(outptr);
                reading_jpg = 0;
            }
            sprintf(filename, "%03i.jpg",count);
            outptr = fopen(filename, "w");
            count +=1;
            reading_jpg = 1;
        }
        if ((reading_jpg == 1)&&(keep_reading == 1))
            {
                fwrite(&buffer, 512, 1, outptr);
            }
    }
    
    
    
    
    
    
    
}
