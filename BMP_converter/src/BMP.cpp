#include "BMP.h"

#include <iostream>
#include <stdio.h>

using namespace std;

BMP::BMP(const char *file_name)
{
    if(picture = fopen(file_name, "rb"))
    {
        fread(&file_info.type,2,1,picture);
        fread(&file_info.fileSize,4,1,picture);
        fread(&file_info.reserved0,2,1,picture);
        fread(&file_info.reserved1,2,1,picture);
        fread(&file_info.dataOffset,4,1,picture);

        fread(&picture_info.hdrSize,4,1,picture);
        fread(&picture_info.width,4,1,picture);
        fread(&picture_info.height,4,1,picture);
        fread(&picture_info.planes,2,1,picture);
        fread(&picture_info.depth,2,1,picture);
        fread(&picture_info.compression,4,1,picture);
        fread(&picture_info.bmpDataSize,4,1,picture);
        fread(&picture_info.hResolution,4,1,picture);
        fread(&picture_info.vResolution,4,1,picture);
        fread(&picture_info.nColors,4,1,picture);
        fread(&picture_info.nImportantColors,4,1,picture);

        bitmap = new Pixel *[picture_info.height];
        for(int i = 0; i < picture_info.height; i++)
            bitmap[i] = new Pixel [picture_info.width];
        if((picture_info.width*3)%4)
            padding = 4-(picture_info.width*3)%4;
        else
            padding = 0;

        fseek(picture, file_info.dataOffset,SEEK_SET);
        for(int i = 0; i < picture_info.height; i++, fseek(picture, padding, SEEK_CUR))
        {
            for(int j = 0; j < picture_info.width; j++)
            {
                bitmap[i][j].B = fgetc(picture);
                bitmap[i][j].G = fgetc(picture);
                bitmap[i][j].R = fgetc(picture);
            }
        }
    }
    else
        cout << "Failed to open file..." << endl;

}

BMP::get_informations()
{

}

BMP::export_picture(const char *result_file_name)
{
    FILE *result;
    if(result = fopen(result_file_name,"wb"))
    {
        fseek(picture, 0, SEEK_END);
        int j = ftell(picture);
        fseek(picture, 0, SEEK_SET);
        for(int i = 0; i < j; i++)
            fputc(fgetc(picture), result);
        char szary;
        fseek(result, file_info.dataOffset, SEEK_SET);
        for(int i = 0; i < picture_info.height; i++, fseek(result, padding, SEEK_CUR))
        {
            for(int j = 0; j < picture_info.width; j++)
            {
                szary = (char)(0.229*bitmap[i][j].R + 0.587*bitmap[i][j].G + 0.114*bitmap[i][j].B);
                fputc(szary, result);
                fputc(szary, result);
                fputc(szary, result);
            }
        }
        fclose(result);
    }
    else
        cout << "Failed to create new file..." << endl;
}

BMP::~BMP()
{
    fclose(picture);
    for(int i = 0; i<picture_info.height; i++)
        delete (bitmap[i]);

    delete(bitmap);
}
