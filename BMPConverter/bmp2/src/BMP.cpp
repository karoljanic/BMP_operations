#include "BMP.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>

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
                bitmap[i][j].RGB_to_HSV();
            }
        }
      /*  bitmap[3][3].R = 0;
        bitmap[3][3].G = 147;
        bitmap[3][3].B = 55;
        cout << "R: " << (int)bitmap[3][3].R << "  " << "G: " << (int)bitmap[3][3].G << "  " << "B: " << (int)bitmap[3][3].B << "  " << endl;
        bitmap[3][3].RGB_to_HSV();
        cout << "Hue: " << bitmap[3][3].hue << "  " << "Saturation: " << bitmap[3][3].saturation << "  " << "Value: " << bitmap[3][3].value << "  " << endl;
        bitmap[3][3].HSV_to_RGB();
        cout << "R: " << (int)bitmap[3][3].R << "  " << "G: " << (int)bitmap[3][3].G << "  " << "B: " << (int)bitmap[3][3].B << "  " << endl;*/
    }
    else
    {
        cout << "Failed to open file..." << endl;
        exit(0);
    }

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
        char kolor;
        fseek(result, file_info.dataOffset, SEEK_SET);
        for(int i = 0; i < picture_info.height; i++,fseek(result, padding, SEEK_CUR))
        {
            for(int j = 0; j < picture_info.width; j++)
            {
                fputc(bitmap[i][j].B, result);
                fputc(bitmap[i][j].G, result);
                fputc(bitmap[i][j].R, result);
            }
        }
        fclose(result);
    }
    else
    {
       cout << "Failed to create new file..." << endl;
       exit(0);
    }

}

BMP::~BMP()
{
    fclose(picture);
    for(int i = 0; i<picture_info.height; i++)
        delete (bitmap[i]);

    delete(bitmap);
}

void BMP::change_brightness()
{
    float b = -1.0;
    cout << "Czy chcesz zwiekszyc jasnosc?" << endl;
    char znak = 0;
    while(znak != 't' && znak != 'n' && znak != 'N' && znak != 'T')
        znak = getch();
    if(znak == 't' || znak == 'T')
        cout << "O ile chcesz zwiekszyc jasnosc?<0 - 100>" << endl;
    else
        cout << "O ile chcesz zmniejszyc jasnosc?<0 - 100>" << endl;
    while(b<0 || b>100)
        cin >> b;
    if(znak == 'n' || znak == 'N')
        b = -b;
    for(int i = 0; i < picture_info.height; i++)
    {
        for(int j = 0; j < picture_info.width; j++)
        {
            bitmap[i][j].value += b/100.0;
            bitmap[i][j].HSV_to_RGB();
        }
    }
}

void BMP::change_saturation()
{
    float s = -1.0;
    cout << "Czy chcesz zwiekszyc nasycenie kolorow?" << endl;
    char znak = 0;
    while(znak != 't' && znak != 'n' && znak != 'N' && znak != 'T')
        znak = getch();
    if(znak == 't' || znak == 'T')
        cout << "O ile chcesz zwiekszyc nasycenie kolorow?<0 - 100>" << endl;
    else
        cout << "O ile chcesz zmniejszyc nasycenie kolorow?<0 - 100>" << endl;
    while(s<0 || s>100)
        cin >> s;
    if(znak == 'n' || znak == 'N')
        s = -s;
    for(int i = 0; i < picture_info.height; i++)
    {
        for(int j = 0; j < picture_info.width; j++)
        {
            bitmap[i][j].saturation += s/100.0;
            bitmap[i][j].HSV_to_RGB();
        }
    }
}

void BMP::change_color_scale()
{
    float h = 200.0;
        cout << "O ile chcesz przesunac skale barw?<-180 - 180>" << endl;
    while(h<-180 || h>180)
        cin >> h;
    for(int i = 0; i < picture_info.height; i++)
    {
        for(int j = 0; j < picture_info.width; j++)
        {
            bitmap[i][j].hue += h;
            bitmap[i][j].HSV_to_RGB();
        }
    }

}

void BMP::negative()
{
    int LUT[256];
    for(int i = 0; i<256; i++)
        LUT[i] = 255-i;
    for(int i = 0; i < picture_info.height; i++)
    {
        for(int j = 0; j < picture_info.width; j++)
        {
            bitmap[i][j].R = LUT[bitmap[i][j].R];
            bitmap[i][j].G = LUT[bitmap[i][j].G];
            bitmap[i][j].B = LUT[bitmap[i][j].B];
        }
    }


}

void BMP::grey_image()
{
    float grey;
    for(int i = 0; i < picture_info.height; i++)
    {
        for(int j = 0; j < picture_info.width; j++)
        {
            grey = (char)(0.229*bitmap[i][j].R + 0.587*bitmap[i][j].G + 0.114*bitmap[i][j].B);
            bitmap[i][j].R = (unsigned char) grey;
            bitmap[i][j].G = (unsigned char) grey;
            bitmap[i][j].B = (unsigned char) grey;
        }
    }
}

void BMP::blur_image()
{

}

void BMP::sharpen_image()
{

}

void BMP::edge_detection()
{

}

void BMP::halftone_approximation()
{

}

void BMP::histogram()
{

}

void BMP::color_accent()
{

}

void BMP::retinex()
{

}

void BMP::tilf_shift()
{

}

void BMP::color_toning()
{

}
