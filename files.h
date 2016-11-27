//
// Created by toxad on 25.11.2016.
//
#ifndef INF_FILES_H
#define INF_FILES_H
#include <stdio.h>
#include <string>
#include <fstream>

namespace bitMap {
    struct BMPHeader {
        unsigned int  bfType;
        unsigned long bfSize;
        unsigned int  bfReserved1;
        unsigned int  bfReserved2;
        unsigned long bfOffBits;
    };

    struct BMPInfoHeader {
        unsigned int    biSize;
        int              biWidth;
        int              biHeight;
        unsigned short  biPlanes;
        unsigned short  biBitCount;
        unsigned int    biCompression;
        unsigned int    biSizeImage;
        int              biXPelsPerMeter;
        int              biYPelsPerMeter;
        unsigned int    biClrUsed;
        unsigned int    biClrImportant;
    };

    struct GreyCount {
        unsigned int num;
        unsigned char tone;
        long double frq;
        std::string uniformCodes;
        std::string codeShannonFano;
        std::string codeHafman;
    };

    struct HafmanTree{
        long double frq;
        int posAlb;
        HafmanTree *left;
        HafmanTree *right;
        HafmanTree ( HafmanTree *l, HafmanTree *r);
        HafmanTree (long double f, int p, HafmanTree *l, HafmanTree *r);
        //bool operator<(const HafmanTree *a, const HafmanTree *b);
    };

    struct Alphabet{
        int num;
        GreyCount *simbol;
    };
}

class BMP{
private:
    bitMap::BMPHeader head;
    bitMap::BMPInfoHeader headInfo;
    unsigned char **field;
    unsigned int shades[256];
    bitMap::Alphabet *alphabet;
    long double entropy;
    int    minBinaryCode;
protected:
    unsigned short read_u16(FILE *fp);
    unsigned int   read_u32(FILE *fp);
    int             read_s32(FILE *fp);
    std::string     getBit(int a);
    std::string     getUmCode(int a, int n);
    void           buildingCodeShannonFano(char lit, std::string code, int left, int right);
    void           buildingCodeHafman();
    void           fun( bitMap::HafmanTree *node, std::string &code);
public:
    BMP( char * );
    ~BMP();
    void writeMatrix();
    void writeInfo();
    void writeFrequency();
    unsigned long   getBitSize();
    int              getWidth();
    int              getHeight();
    unsigned short  getBitCount();
    unsigned int    getCompression();
    unsigned int    getSizeImage();
    int              getXPelsPerMeter();
    int              getYPelsPerMeter();
    unsigned int    getClrUsed();
    unsigned int    getClrImportant();
    long double     getEntropy();
    int              getminBinaryCode();
    void             getUniformCodes( std::ofstream &out );
    void             getCodeShannonFano( std::ofstream &out );
    void             getCodeHafman( std::ofstream &out );
    long double     byteSizeCodeUniform();
    long double     byteSizeCodeShannonFano();
    long double     byteSizeCodeHafman();
    long int        sizeCodeUniform();
    long int        sizeCodeShannonFano();
    long int        sizeCodeHafman();

};
#endif //INF_FILES_H
