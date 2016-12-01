#ifndef INF_FILES_H
#define INF_FILES_H
#include <stdio.h>
#include <string>
#include <fstream>

namespace bitMap {
    struct BMPHeader { //Информация об изображении
        unsigned int    bfType;
        unsigned long   bfSize;
        unsigned int    bfReserved1;
        unsigned int    bfReserved2;
        unsigned long   bfOffBits;
    };

    struct BMPInfoHeader { //Информация об изображении
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

    struct GreyCount { //Информация о каждом символе первичного алфавита и его кодах
        unsigned int    num;
        unsigned char   tone;
        long double     frq;
        std::string     uniformCodes;
        std::string     codeShannonFano;
        std::string     codeHuffman;
    };

    struct HuffmanTree{ //Дерево Хаффамана
        long double     frq;
        int             posAlb;
        HuffmanTree     *left;
        HuffmanTree     *right;
        HuffmanTree     ( HuffmanTree *l, HuffmanTree *r );
        HuffmanTree     ( long double f, int p, HuffmanTree *l, HuffmanTree *r );
    };

    struct Alphabet{ //Алфавит из значений символов
        int             num;
        GreyCount       *symbol;
    };
}

enum typeOfCode{ //Типы кодов
    UNIFORM, //Равномерный
    HUFFMAN, //По Хаффману
    SHANNON  //По Шеннону-Фано
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
    unsigned short        read_u16                  ( FILE *fp ); //побитовое чтение 16 бит
    unsigned int          read_u32                  ( FILE *fp ); //побитовое чтение 32 бит
    int                   read_s32                  ( FILE *fp ); //побитовое чтение 31 бита
    std::string           getBit                    ( int a );    //получение двоичного представления числа
    std::string           getUmCode                 ( int a, int n ); //получение равномерного кода числа
    void                  buildingCodeShannonFano   ( char lit, std::string code, int left, int right ); //построение кодов Шеннона-Фано
    void                  buildingCodeHuffman       (); //Построение кодов Хаффмана
    void                  fun                       ( bitMap::HuffmanTree *node, std::string &code ); //Вспомогательная функция построения дерева Хаффмана
    int                   correlation               (int a); //Квантование
public:
    BMP                                             ( char * );
    ~BMP                                            ();
    void                  writeMatrix               ( std::ofstream *out = nullptr ); //Вывод матрицы изображения
    void                  writeInfo                 ( std::ofstream *out = nullptr ); //Вывод информации изображения
    void                  writeFrequencyProbability ( std::ofstream *out = nullptr ); //Вывод вероятностоной частоты
    void                  writeFrequency            ( std::ofstream *out = nullptr ); //Вывод количественной частоты
    unsigned long         getBitSize                (); //Получения значения информации об изображении
    int                   getWidth                  (); //Получения значения информации об изображении
    int                   getHeight                 (); //Получения значения информации об изображении
    unsigned short        getBitCount               (); //Получения значения информации об изображении
    unsigned int          getCompression            (); //Получения значения информации об изображении
    unsigned int          getSizeImage              (); //Получения значения информации об изображении
    int                   getXPelsPerMeter          (); //Получения значения информации об изображении
    int                   getYPelsPerMeter          (); //Получения значения информации об изображении
    unsigned int          getClrUsed                (); //Получения значения информации об изображении
    unsigned int          getClrImportant           (); //Получения значения информации об изображении
    long double           getEntropy                (); //Получения значения информации об изображении
    long double           byteSizeCode              ( typeOfCode ); //Значение энтропии по Шеннону
    long int              sizeCode                  ( typeOfCode ); //Размер кода
    void                  writeCodesOfMessage       ( typeOfCode, std::ofstream *out = nullptr ); //Вывод закодированного сообщения
    void                  writeCodesOfSymbols       ( typeOfCode, std::ofstream *out =  nullptr ); //Вывод кодов в соответствии с методом сжатия
};
#endif //INF_FILES_H
