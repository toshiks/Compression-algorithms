//
// Created by toxad on 26.11.2016.
//
#include "files.h"
#include <cmath>
#include <iostream>
#include <iomanip>

BMP::BMP( char * )
{
    for ( int i = 0; i < 256; i++)
        shades[i] = 0;
    using namespace bitMap;
    FILE *f = fopen("D:\\Git\\INF\\src.bmp","rb");

    head = new BMPHeader;
    headInfo = new BMPInfoHeader;

    read_16( f, head->bfType );
    read_32( f, head->bfSize );
    read_16( f, head->bfReserved1);
    read_16( f, head->bfReserved2);
    read_32( f, head->bfOffBits);

    read_32( f, headInfo->biSize);
    read_32( f, headInfo->biWidth);
    read_32( f, headInfo->biHeight);
    read_16( f, headInfo->biPlanes);
    read_16( f, headInfo->biBitCount);
    read_32( f, headInfo->biCompression);
    read_32( f, headInfo->biSizeImage);
    read_32( f, headInfo->biXPelsPerMeter);
    read_32( f, headInfo->biYPelsPerMeter);
    read_32( f, headInfo->biClrUsed);
    read_32( f, headInfo->biClrImportant);

    field = new GreyField* [headInfo->biWidth];
    for ( int i = 0; i < headInfo->biWidth; i++ ){
        field[i] = new GreyField [headInfo->biHeight];
    }

    int coun = 0;

    for ( int i = 0; i < headInfo->biWidth; i++ ){
        for ( int j = 0; j < headInfo->biHeight; j++ ) {
            field[i][j].tone = getc( f );

            if ( shades[ int( std::round( field[i][j].tone / 20 ) * 20) ] == 0 )
                coun++;

            shades[ int( std::round( field[i][j].tone / 20 ) * 20) ]++;
        }
        getc( f );
    }

    alphabet = new Alphabet;
    alphabet->num = coun;
    alphabet->simbol = new GreyCount [ alphabet->num ];

    coun = 0;
    for ( unsigned short i = 0; i < 256; i++ ){
        if ( shades[i] != 0 ) {
            alphabet->simbol[coun].tone = i;
            alphabet->simbol[coun].num  = shades[i];
            coun++;
        }
    }
}

BMP::~BMP()
{
    using namespace bitMap;

    delete[] alphabet->simbol;
    delete[] alphabet;

    for ( int i = 0; i < headInfo->biWidth; i++ ){
        delete[] field[i];
    }
    delete[] field;

    delete headInfo;

    delete head;
}


void BMP::writeMatrix()
{
    using namespace std;

    for ( int i = 0; i < headInfo->biWidth; i++ ){
        for ( int j = 0; j < headInfo->biHeight; j++ ) {
            cout << setw(3) << setfill('0') <<  field[i][j].tone << ' ' ;
        }
        cout << endl;
    }
    cout << endl;
}

void BMP::writeInfo()
{
    using namespace std;

    printf("General Information of BMP Files\n");
    printf("bfType: %c\n",head->bfType);
    printf("bfSize: %d\n",head->bfSize);
    printf("bfReserved1: %d\n",head->bfReserved1);
    printf("bfReserved2: %d\n",head->bfReserved2);
    printf("bfOffBits: %d\n",head->bfOffBits);

    printf("\nHeader Information of BMP Files\n");
    printf("biSize: %d\n",headInfo->biSize);
    printf("biWidth: %u\n",headInfo->biWidth);
    printf("biHeight: %u\n",headInfo->biHeight);
    printf("biPlanes: %d\n",headInfo->biPlanes);
    printf("biBitCount: %d\n",headInfo->biBitCount);
    printf("biCompression: %d\n",headInfo->biCompression);
    printf("biSizeImage: %d\n",headInfo->biSizeImage);
    printf("biXPelsPerMeter: %u\n",headInfo->biXPelsPerMeter);
    printf("biYPelsPerMeter: %u\n",headInfo->biYPelsPerMeter);
    printf("biClrUsed: %d\n",headInfo->biClrUsed);
    printf("biClrImportant: %d\n\n",headInfo->biClrImportant);
}

template <typename K>
void BMP::read_16(FILE *fp, K &temp)
{
    unsigned char b0, b1;

    b0 = getc( fp );
    b1 = getc( fp );

    temp =  ( K )( ( b0 << 8 ) | b1 );
}

template <typename T>
void BMP::read_32(FILE *fp, T &temp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc( fp );
    b1 = getc( fp );
    b2 = getc( fp );
    b3 = getc( fp );

    temp = ( ( T ) ( ( ( ( ( b3 << 8 ) | b2 ) << 8 ) | b1 ) << 8 ) | b0 );
}

