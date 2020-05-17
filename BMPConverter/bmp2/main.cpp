#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include<string>
#include "BMP.h"

using namespace std;

void wyswietl(string *instrukcje, int dlugosc_tablicy, int obecna_pozycja)
{
    cout << "***  Edytor plikow BMP   ***" << endl << endl;
    cout << "_______Wybierz operacje_______" << endl;

    for(int i = 0; i<dlugosc_tablicy; i++)
    {
        if(i == obecna_pozycja)
            cout << "-->" << instrukcje[i] << "<--" << endl;
        else
            cout << "   " << instrukcje[i] << endl;
    }
}

int main()
{
    int operacja = 0;
    char z = 0;
    int n = 14;
    string opcje[] = {"    Zmiana jasnosci     ",
                      "Zmiana nasycenia kolorow",
                      "  Zmiana skali kolorow  ",
                      "        Negatyw         ",
                      "    Obraz w szarosci    ",
                      "        Rozmycie        ",
                      "      Wyostrzenie       ",
                      "  Wykrywanie krawedzi   ",
                      " Aproksymacja poltonowa ",
                      "        Histogram       ",
                      "  Akcent kolorystyczny  ",
                      "    Algorytm Retinex    ",
                      "    Efekt Tilt-Shift    ",
                      "        Tonowanie       "};

    wyswietl(opcje,n,operacja);
    while(z != 13)
    {
        z = getch();
        if(z == -32)
        {
            z = getch();
            if(z == 72 && operacja>0)
                operacja--;
            else if(z == 80 && operacja<n-1)
                operacja++;
        }
        system("cls");
        wyswietl(opcje,n,operacja);
    }
    system("cls");
    cout << "Podaj nazwe pliku wraz z jego rozszerzeniem!" << endl;
    const char *nazwa1;
    string nazwa;
    while(nazwa == "")
        cin >> nazwa;
    nazwa1 = nazwa.c_str();
    BMP photo(nazwa1);

    system("cls");
    cout << "Podaj nazwe pliku wyjsciowego wraz z jego rozszerzeniem!" << endl;
    nazwa = "";
    while(nazwa == "")
        cin >> nazwa;
        nazwa1 = nazwa.c_str();

    switch(operacja)
    {
    case 0:
        system("cls");
        photo.change_brightness();
        break;
    case 1:
        system("cls");
        photo.change_saturation();
        break;
    case 2:
        system("cls");
        photo.change_color_scale();
        break;
    case 3:
        system("cls");
        photo.negative();
        break;
    case 4:
        system("cls");
        photo.grey_image();
        break;
    case 5:
        photo.blur_image();
        break;
    case 6:
        photo.sharpen_image();
        break;
    case 7:
        photo.edge_detection();
        break;
    case 8:
        photo.halftone_approximation();
        break;
    case 9:
        photo.histogram();
        break;
    case 10:
        photo.color_accent();
        break;
    case 11:
        photo.retinex();
        break;
    case 12:
        photo.tilf_shift();
    case 13:
        photo.color_toning();
    }

    photo.export_picture(nazwa1);

    return 0;

}
