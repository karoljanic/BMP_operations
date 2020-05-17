#include <stdio.h>
#include <windows.h>
#include <iostream>
#include "BMP.h"

using namespace std;

int main()
{
    BMP photo1("test.bmp");
    photo1.export_picture("photo1.bmp");


    return 0;

}


