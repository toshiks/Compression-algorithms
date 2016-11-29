#ifndef INF_FILES_H
#define INF_FILES_H
#include <stdio.h>
#include <string>
#include <fstream>

namespace bitMap {
    struct BMPHeader {
        unsigned int    bfType;
        unsigned long   bfSize;
        unsigned int    bfReserved1;
        unsigned int    bfReserved2;
        unsigned long   bfOffBits;
    };

    struct BMPInfoHeader {
        unsigned int    biSize;
        int             biWidth;
        int             biHeight;
        unsigned short  biPlanes;
        unsigned short  biBitCount;
        unsigned int    biCompression;
        unsigned int    biSizeImage;
        int             biXPelsPerMeter;
        int             biYPelsPerMeter;
        unsigned int    biClrUsed;
        unsigned int    biClrImportant;
    };

    struct GreyCount {
        unsigned int    num;
        unsigned char   tone;
        long double     frq;
        std::string     uniformCodes;
        std::string     codeShannonFano;
        std::string     codeHafman;
    };

    struct HuffmanTree{
        long double     frq;
        int             posAlb;
        HuffmanTree     *left;
        HuffmanTree     *right;
        HuffmanTree     ( HuffmanTree *l, HuffmanTree *r );
        HuffmanTree     ( long double f, int p, HuffmanTree *l, HuffmanTree *r );
    };

    struct Alphabet{
        int             num;
        GreyCount       *simbol;
    };
}

enum typeOfCode{
    UNIFORM,
    HUFFMAN,
    SHENNON
};

class BMP{
private:
    bitMap::BMPHeader     head;
    bitMap::BMPInfoHeader headInfo;
    unsigned char         **field;
    unsigned int          shades[256];
    bitMap::Alphabet      *alphabet;
    long double           entropy;
protected:
    unsigned short        read_u16                  ( FILE *fp );
    unsigned int          read_u32                  ( FILE *fp );
    int                   read_s32                  ( FILE *fp );
    std::string           getBit                    ( int a );
    std::string           getUmCode                 ( int a, int n );
    void                  buildingCodeShannonFano   ( char lit, std::string code, int left, int right );
    void                  buildingCodeHuffman       ();
    void                  fun                       ( bitMap::HuffmanTree *node, std::string &code );
    int                   correlation               (int a);
public:
    BMP                                             ( char * );
    ~BMP                                            ();
    void                  writeMatrix               ( std::ofstream *out = nullptr );
    void                  writeInfo                 ( std::ofstream *out = nullptr );
    void                  writeFrequencyProbability ( std::ofstream *out = nullptr );
    void                  writeFrequency            ( std::ofstream *out = nullptr );
    unsigned long         getBitSize                ();
    int                   getWidth                  ();
    int                   getHeight                 ();
    unsigned short        getBitCount               ();
    unsigned int          getCompression            ();
    unsigned int          getSizeImage              ();
    int                   getXPelsPerMeter          ();
    int                   getYPelsPerMeter          ();
    unsigned int          getClrUsed                ();
    unsigned int          getClrImportant           ();
    long double           getEntropy                ();
    long double           byteSizeCode              ( typeOfCode );
    long int              sizeCode                  ( typeOfCode );
    void                  writeCodesOfMessage       ( typeOfCode, std::ofstream *out = nullptr );
    void                  writeCodesOfSymbols       ( typeOfCode, std::ofstream *out =  nullptr );

};
#endif //INF_FILES_H
