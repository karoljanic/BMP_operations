#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <stdio.h>

using namespace std;

class BMP
{
     private:
        struct BMPFILEHEADER
        {
            short type;
            int fileSize;
            short reserved0;
            short reserved1;
            int dataOffset;
        };

        struct BMPINFOHEADER
        {
            int hdrSize;
            int width;
            int height;
            short planes;
            short depth;
            int compression;
            int bmpDataSize;
            int hResolution;
            int vResolution;
            int nColors;
            int nImportantColors;
        };

        struct Pixel
        {
            unsigned char R;
            unsigned char G;
            unsigned char B;
        };

        FILE *picture;
        BMPFILEHEADER file_info;
        BMPINFOHEADER picture_info;

        int padding;

    public:
        BMP(const char *file_name);
        get_informations();
        export_picture(const char *result_file_name);
        ~BMP();

        Pixel **bitmap;

};

#endif // BMP_H
