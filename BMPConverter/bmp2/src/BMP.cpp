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

void BMP::filter(int **filter, int filter_size, int sum_of_filter, int i_p, int j_p)
{

    int m, n,f, sumR = 0, sumG = 0, sumB = 0;
    int i = i_p -(filter_size-1)/2;
    int j = j_p -(filter_size-1)/2;

    for(int k = 0; k<filter_size; k++)
    {
        for(int l = 0; l<filter_size; l++)
        {
            m = i+k;
            n = j+l;

            if(m<0)
                m = 0;
            else if(m>picture_info.height-1)
                m = picture_info.height-1;

            if(n<0)
                n = 0;
            else if(n>picture_info.width-1)
                n = picture_info.width-1;

            sumR += int(bitmap[m][n].R)*filter[k][l];
            sumG += int(bitmap[m][n].G)*filter[k][l];
            sumB += int(bitmap[m][n].B)*filter[k][l];

        }
    }

    sumR/=sum_of_filter;
    sumG/=sum_of_filter;
    sumB/=sum_of_filter;

    if(sumR<0)
        sumR = 0;
    else if(sumR>255)
        sumR = 255;

    if(sumG<0)
        sumG = 0;
    else if(sumG>255)
        sumG = 255;

    if(sumB<0)
        sumB = 0;
    else if(sumB>255)
        sumB = 255;

    bitmap2[i_p][j_p].R = sumR;
    bitmap2[i_p][j_p].G = sumG;
    bitmap2[i_p][j_p].B = sumB;

}

void BMP::blur_image()
{
    bitmap2 = new Pixel *[picture_info.height];
    for(int i = 0; i < picture_info.height; i++)
        bitmap2[i] = new Pixel [picture_info.width];

    int **f;
    /*f = new int*[3];
    for(int i = 0; i<3; i++)
    {
        f[i] = new int[3];
    }
    f[1][1] = 4;
    f[0][0] = 1, f[0][2] = 1, f[2][0] = 1, f[2][2] = 1;
    f[0][1] = 2, f[1][0] = 2, f[1][2] = 2, f[2][1] = 2;
    */

    f = new int*[8];
    for(int i = 0; i<8; i++)
        f[i] = new int[8];

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
            f[i][j] = 1;
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            filter(f, 8, 64, i, j);
        }
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            bitmap[i][j] = bitmap2[i][j];
        }
    }
}

void BMP::sharpen_image()
{
    bitmap2 = new Pixel *[picture_info.height];
    for(int i = 0; i < picture_info.height; i++)
        bitmap2[i] = new Pixel [picture_info.width];

    int **f;
    f = new int*[3];
    for(int i = 0; i<3; i++)
    {
        f[i] = new int[3];
    }
    f[1][1] = 5;
    f[0][0] = 0, f[0][2] = 0, f[2][0] = 0, f[2][2] = 0;
    f[0][1] = -1, f[1][0] = -1, f[1][2] = -1, f[2][1] = -1;

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            filter(f, 3, 1, i, j);
        }
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            bitmap[i][j] = bitmap2[i][j];
        }
    }
}

void BMP::edge_detection()
{
    bitmap2 = new Pixel *[picture_info.height];
    for(int i = 0; i < picture_info.height; i++)
        bitmap2[i] = new Pixel [picture_info.width];

    int **f;
    f = new int*[3];
    for(int i = 0; i<3; i++)
    {
        f[i] = new int[3];
    }
    f[1][1] = 4;
    f[0][0] = 0, f[0][2] = 0, f[2][0] = 0, f[2][2] = 0;
    f[0][1] = -1, f[1][0] = -1, f[1][2] = -1, f[2][1] = -1;

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            filter(f, 3, 1, i, j);
        }
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            bitmap[i][j] = bitmap2[i][j];
        }
    }
}

void BMP::halftone_approximation()
{
    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            if((bitmap[i][j].R + bitmap[i][j].G + bitmap[i][j].B)<381)
            {
                bitmap[i][j].R = 0;
                bitmap[i][j].G = 0;
                bitmap[i][j].B = 0;
            }
            else
            {
                bitmap[i][j].R = 255;
                bitmap[i][j].G = 255;
                bitmap[i][j].B = 255;
            }
        }
    }

}

void BMP::color_accent()
{
    cout << "Wybierz kat odpowiadajacy barwie w modelu HSV, ktora checesz zaakcentowac!" << endl;
    cout << "<0 - 360>(np.0 to czerowny, 120 to zielony, 240 to niebieski)" << endl;
    float h = 666, r = -7;
    while(h<0 || h>360)
        cin >> h;
    cout << "Podaj zakres akcentowanego koloru!<0 - 360>"<< endl;
    while(r<0 || r>360)
        cin >> r;

    int h1 = int(h-r/2+360)%360;
    int h2 = int(h+r/2+360)%360;

    float grey;

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            if(!(( h1 <= h2 && h1 <= bitmap[i][j].hue && h2 >= bitmap[i][j].hue) ||( h1 > h2 && ( h1 <= bitmap[i][j].hue || h2 >= bitmap[i][j].hue ))))
            {
                grey = (char)(0.229*bitmap[i][j].R + 0.587*bitmap[i][j].G + 0.114*bitmap[i][j].B);
                bitmap[i][j].R = (unsigned char) grey;
                bitmap[i][j].G = (unsigned char) grey;
                bitmap[i][j].B = (unsigned char) grey;
            }
        }
    }
}

void BMP::retinex()
{

}

void BMP::tilf_shift()
{
    bitmap2 = new Pixel *[picture_info.height];
    for(int i = 0; i < picture_info.height; i++)
        bitmap2[i] = new Pixel [picture_info.width];

    int **f;
    f = new int*[12];
    for(int i = 0; i<12; i++)
        f[i] = new int[12];

    for(int i=0; i<12; i++)
    {
        for(int j=0; j<12; j++)
            f[i][j] = 1;
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            filter(f, 12, 144, i, j);
        }
    }
    float a,b,c,h;
    cout << "Podaj 3 liczby odpowiadajace stosunkowi wydzielonych przez rozmycie czesci(np.: 3 2 3)" << endl;
    cin >> a >> b >> c;
    h = a+b+c;
    h /= picture_info.height;
    int h1, h2;
    h1 = a/h;
    h2 = (a+b)/h;

    for(int i = 0; i<h1; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
            bitmap[i][j] = bitmap2[i][j];
    }

    for(int i = h2; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
            bitmap[i][j] = bitmap2[i][j];
    }


}

void BMP::color_toning()
{
    float h = 666;
    cout << "Wybierz kat odpowiadajacy barwie w modelu HSV, ktora chesz tonowac!" << endl;
    cout << "<0 - 360>(np.0 to czerowny, 120 to zielony, 240 to niebieski)" << endl;
    while(h<0 || h>360)
        cin >> h;

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
               bitmap[i][j].hue = h;
               bitmap[i][j].HSV_to_RGB();
        }
    }

}

void BMP::filtr()
{
    bitmap2 = new Pixel *[picture_info.height];
    for(int i = 0; i < picture_info.height; i++)
        bitmap2[i] = new Pixel [picture_info.width];

    int n =2;
    cout << "Podaj szerokosc filtra!Pamietaj aby byla ona liczba nieparzysta"<< endl;
    while(n%2 != 1)
        cin >> n;

    int **f;
    f = new int*[n];
    for(int i = 0; i<n; i++)
        f[i] = new int[n];
    cout << "Podawaj kolejne wartosci filtra. Rozpoczynajac o lewego gornego elementu, a konczac na prawym dolnym!" << endl;
    int suma = 0;
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<n; j++)
        {
            cin >> f[i][j];
            suma += f[i][j];
        }
    }

    if(suma == 0)
        suma = 1;

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            filter(f, n, suma, i, j);
        }
    }

    for(int i = 0; i<picture_info.height; i++)
    {
        for(int j = 0; j<picture_info.width; j++)
        {
            bitmap[i][j] = bitmap2[i][j];
        }
    }


}
