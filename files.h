//
// Created by toxad on 25.11.2016.
//

#ifndef INF_FILES_H
#define INF_FILES_H
#include <stdio.h>

namespace bitMap {
    struct BMPHeader {
        unsigned int bfType;
        unsigned long bfSize;
        unsigned int bfReserved1;
        unsigned int bfReserved2;
        unsigned long bfOffBits;
    };

    struct BMPInfoHeader {
        unsigned int biSize;
        int biWidth;
        int biHeight;
        unsigned short biPlanes;
        unsigned short biBitCount;
        unsigned int biCompression;
        unsigned int biSizeImage;
        int biXPelsPerMeter;
        int biYPelsPerMeter;
        unsigned int biClrUsed;
        unsigned int biClrImportant;
    };

    struct GreyField {
        unsigned int tone;
        int rgbReserved;
    };

    struct GreyCount {
        unsigned int num;
        unsigned short tone;
    };

    struct Alphabet{
        int num;
        GreyCount *simbol;
    };
}

class BMP{
private:
    bitMap::BMPHeader *head;
    bitMap::BMPInfoHeader *headInfo;
    bitMap::GreyField **field;
    unsigned int shades[256];
    bitMap::Alphabet *alphabet;
protected:
    template <typename K>
    void read_16(FILE *fp, K &temp);
    template <typename T>
    void read_32(FILE *fp, T &temp);

public:
    BMP( char * );
    ~BMP();
    void writeMatrix();
    void writeInfo();
};

#endif //INF_FILES_H
