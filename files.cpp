//
// Created by toxad on 26.11.2016.
//
#include "files.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <map>

/*bool bitMap::HafmanTree::operator<(const HafmanTree *a, const HafmanTree *b)
{
    return a->frq < b->frq;
}*/

bitMap::HafmanTree::HafmanTree(HafmanTree *l, HafmanTree *r)
{
    left = l;
    right = r;
    frq = l->frq + r->frq;
    posAlb = -1;
}

bitMap::HafmanTree::HafmanTree(long double f, int p, HafmanTree *l, HafmanTree *r)
{
    frq = f;
    posAlb = p;
    left = l;
    right = r;
}

int correlation(int a)
{
    return int(std::round(a/20)*20);
}

BMP::BMP( char * fileName )
{
    using namespace bitMap;
    FILE *f = fopen( fileName, "rb" );

    head.bfType = read_u16( f );
    head.bfSize = read_u32( f );
    head.bfReserved1 = read_u16( f );
    head.bfReserved2 = read_u16( f );
    head.bfOffBits = read_u32( f );

    headInfo.biSize = read_u32( f );
    headInfo.biWidth = read_s32( f );
    headInfo.biHeight = read_s32( f );
    headInfo.biPlanes = read_u16( f );
    headInfo.biBitCount = read_u16( f );
    headInfo.biCompression = read_u32( f );
    headInfo.biSizeImage = read_u32( f );
    headInfo.biXPelsPerMeter = read_s32( f );
    headInfo.biYPelsPerMeter = read_s32( f );
    headInfo.biClrUsed = read_u32( f );
    headInfo.biClrImportant = read_u32( f );

    field = new unsigned char* [headInfo.biWidth];
    for (int i =0; i < headInfo.biWidth; i++){
        field[i] = new unsigned char[headInfo.biHeight];
    }

    std::fill(shades, shades+256, 0);

    int coun = 0;
    for ( int i = 0; i < headInfo.biWidth; i++ ){
        for ( int j = 0; j < headInfo.biHeight; j++ ) {
            field[i][j] = (unsigned char) getc(f);
            if (shades[correlation(field[i][j])] == 0)
                coun++;

            shades[correlation(field[i][j])]++;

        }
        getc( f );
    }

    alphabet = new Alphabet;
    alphabet->num = coun;
    alphabet->simbol = new GreyCount [ alphabet->num ];

    coun = 0;
    int sum = 0;
    for ( auto i = 0; i < 256; i++ ){
        if ( shades[i] != 0 ) {
            alphabet->simbol[coun].tone = (unsigned char)i;
            alphabet->simbol[coun].num  = shades[i];
            sum += shades[i];
            coun++;
        }
    }

    minBinaryCode = int( std::ceil( (long double)std::log2( alphabet->num ) ) );

    entropy = 0;
    for ( auto i = 0; i < alphabet->num; i++ ){
        alphabet->simbol[i].frq = (double)alphabet->simbol[i].num / (double)sum;
        entropy = entropy + (alphabet->simbol[i].frq * std::log2( alphabet->simbol[i].frq ));
    }

    entropy *= (-1);


    for ( int i = 0; i < alphabet->num; i++ ){
        alphabet->simbol[i].uniformCodes = getUmCode( i, minBinaryCode );
        std::cout << (int)alphabet->simbol[i].tone << ": " << alphabet->simbol[i].uniformCodes << '\n';
    }

    std::cout << '\n';
    std::sort(alphabet->simbol, alphabet->simbol+alphabet->num, [](GreyCount a, GreyCount b){ return a.frq < b.frq;} );
    buildingCodeShannonFano('4', "-", 0, alphabet->num-1);
    std::cout << '\n';
    for ( int i = 0; i < alphabet->num; i++ ){
        std::cout << (int)alphabet->simbol[i].tone << ": " << alphabet->simbol[i].codeShannonFano << '\n';
    }

    buildingCodeHafman();
    std::cout << '\n';
    for ( int i = 0; i < alphabet->num; i++ ){
        std::cout << (int)alphabet->simbol[i].tone << ": " << alphabet->simbol[i].codeHafman << '\n';
    }

    fclose(f);
}


void BMP::buildingCodeShannonFano(char lit, std::string code, int left, int right)
{
    if ( lit != '1' && lit != '0' )
        code = "";
    else
        code += lit;

    if ( left == right ) {
        alphabet->simbol[left].codeShannonFano = code;
    }
    else{
        long double midFrq = 0;
        for ( int i = left; i < right; i++){
            midFrq += alphabet->simbol[i].frq;
        }

        midFrq /= 2;

        int medium = left;
        long double temp = 0;

        while ( medium < right && temp +alphabet->simbol[medium].frq < midFrq){
            temp += alphabet->simbol[medium].frq;
            medium++;
        }

        buildingCodeShannonFano('1', code, left, medium);
        buildingCodeShannonFano('0', code, medium + 1, right);
    }
}

void BMP::fun( bitMap::HafmanTree *node, std::string &code)
{
    if ( node->left != nullptr ){
        code += '0';
        fun(node->left, code);
    }
    if ( node->right != nullptr ){
        code += '1';
        fun( node->right, code );
    }

    if (node->posAlb != -1)
        alphabet->simbol[node->posAlb].codeHafman = code;

	if (!code.empty())
		code.pop_back();
}

void del( bitMap::HafmanTree *node )
{
    if ( node->left != nullptr ){
        del(node->left);
    }
    if ( node->right != nullptr ){
        del(node->right);
    }

    delete node;
}

void BMP::buildingCodeHafman()
{
    using namespace bitMap;
    using namespace std;

    auto comp = [](HafmanTree *a, HafmanTree*b){ return a->frq > b->frq;};

    priority_queue<HafmanTree*, vector<HafmanTree*>, decltype( comp ) > qu(comp);

    for ( int i = 0 ; i < alphabet->num; i++ ){
        HafmanTree *obj = new HafmanTree(alphabet->simbol[i].frq, i, nullptr, nullptr);
        alphabet->simbol[i].codeHafman = "";
        qu.push( obj );
    }


    while( qu.size() > 1 ){
        HafmanTree *l = qu.top();
        qu.pop();
        HafmanTree *r = qu.top();
        qu.pop();

        HafmanTree *root = new HafmanTree(l ,r);

        qu.push(root);
    }

    HafmanTree *root = qu.top();

    string code = "";

    fun( root, code );
    del(root);
}

BMP::~BMP()
{
    using namespace bitMap;

    delete[] alphabet->simbol;
    delete[] alphabet;

    for ( int i = 0; i < headInfo.biWidth; i++ ){
        delete[] field[i];
    }
    delete[] field;
}


std::string BMP::getUmCode(int a, int n)
{
    using namespace std;

    string x = getBit(a);

    reverse( x.begin(), x.end() );

    while ( x.size() != n)
        x += '0';

    reverse( x.begin(), x.end() );

    return x;
}

std::string BMP::getBit(int a)
{
    using namespace std;

    string x = "";
    while (a != 0){
        if ( a & 1 ){
            x += '1';
        }
        else
            x += '0';
        a = a >> 1;

    }

    reverse( x.begin(), x.end() );

    return x;
}

void BMP::writeMatrix()
{
    using namespace std;

    for (int i =0; i < headInfo.biWidth; i++){
        for ( int j =0; j < headInfo.biHeight; j++){
            cout << setw(3) << setfill('0') << field[i][j] << ' ' ;
        }
        cout << endl;
    }
    cout << endl;
}

void BMP::writeFrequency()
{
    using namespace std;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [](bitMap::GreyCount a){
        cout <<  setw(6) << fixed << setprecision(5) << (int)a.tone << ": " << a.frq << endl;
    });
}

void BMP::writeInfo()
{
    using namespace std;

    printf("General Information of BMP Files\n");
    printf("bfType: %c\n",head.bfType);
    printf("bfSize: %lu\n",head.bfSize);
    printf("bfReserved1: %d\n",head.bfReserved1);
    printf("bfReserved2: %d\n",head.bfReserved2);
    printf("bfOffBits: %lu\n",head.bfOffBits);

    printf("\nHeader Information of BMP Files\n");
    printf("biSize: %d\n",headInfo.biSize);
    printf("biWidth: %u\n",headInfo.biWidth);
    printf("biHeight: %u\n",headInfo.biHeight);
    printf("biPlanes: %d\n",headInfo.biPlanes);
    printf("biBitCount: %d\n",headInfo.biBitCount);
    printf("biCompression: %d\n",headInfo.biCompression);
    printf("biSizeImage: %d\n",headInfo.biSizeImage);
    printf("biXPelsPerMeter: %u\n",headInfo.biXPelsPerMeter);
    printf("biYPelsPerMeter: %u\n",headInfo.biYPelsPerMeter);
    printf("biClrUsed: %d\n",headInfo.biClrUsed);
    printf("biClrImportant: %d\n\n",headInfo.biClrImportant);
}

int BMP::getminBinaryCode()
{
    return minBinaryCode;
}

unsigned long   BMP::getBitSize()
{
    return head.bfSize;
}

int BMP::getWidth()
{
    return headInfo.biWidth;
}

int BMP::getHeight()
{
    return headInfo.biHeight;
}

unsigned short BMP::getBitCount()
{
    return headInfo.biBitCount;
}

unsigned int BMP::getCompression()
{
    return headInfo.biCompression;
}

unsigned int BMP::getSizeImage()
{
    return headInfo.biSizeImage;
}

int BMP::getXPelsPerMeter()
{
    return headInfo.biXPelsPerMeter;
}

int BMP::getYPelsPerMeter()
{
    return headInfo.biYPelsPerMeter;
}

unsigned int BMP::getClrUsed()
{
    return headInfo.biClrUsed;
}

unsigned int BMP::getClrImportant()
{
    return headInfo.biClrImportant;
}

long double BMP::getEntropy()
{
    return entropy;
}

void BMP::getUniformCodes( std::ofstream &out )
{
    using namespace std;
    using namespace bitMap;
    map<int, string> dic;

    for (int i =0;i < alphabet->num; i++){
        dic.insert(make_pair((int)alphabet->simbol[i].tone, alphabet->simbol[i].uniformCodes));
    }

    for ( int i = 0; i < headInfo.biWidth; i++ ) {
        for (int j = 0; j < headInfo.biHeight; j++) {
            int t = correlation(field[i][j]);
            out << dic[t];
        }
    }
    out << "\n\n";
}

void BMP::getCodeShannonFano( std::ofstream &out )
{
    using namespace std;
    using namespace bitMap;
    map<int, string> dic;

    for (int i =0;i < alphabet->num; i++){
        dic.insert(make_pair((int)alphabet->simbol[i].tone, alphabet->simbol[i].codeShannonFano));
    }

    for ( int i = 0; i < headInfo.biWidth; i++ ) {
        for (int j = 0; j < headInfo.biHeight; j++) {
            int t = correlation(field[i][j]);
            out << dic[t];
        }
    }
    out << "\n\n";
}


long int BMP::sizeCodeUniform()
{
    using namespace std;
    using namespace bitMap;
    long int temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&](GreyCount a){
        temp += ( a.num * a.uniformCodes.size() );
    });

    return temp;
}

long int BMP::sizeCodeShannonFano()
{
    using namespace std;
    using namespace bitMap;
    long int temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&](GreyCount a){
        temp += ( a.num * a.codeShannonFano.size() );
    });

    return temp;
}

long int BMP::sizeCodeHafman()
{
    using namespace std;
    using namespace bitMap;
    long int temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&](GreyCount a){
        temp += ( a.num * a.codeHafman.size() );
    });

    return temp;
}

void BMP::getCodeHafman( std::ofstream &out)
{
    using namespace std;
    using namespace bitMap;
    map<int, string> dic;

    for (int i =0;i < alphabet->num; i++){
        dic.insert(make_pair((int)alphabet->simbol[i].tone, alphabet->simbol[i].codeHafman));
    }

    for ( int i = 0; i < headInfo.biWidth; i++ ) {
        for (int j = 0; j < headInfo.biHeight; j++) {
            int t = correlation(field[i][j]);
            out << dic[t];
        }
    }
    out << "\n\n";
}

long double BMP::byteSizeCodeUniform()
{
    using namespace std;
    long double temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&temp](bitMap::GreyCount a){
        temp += (a.frq * a.uniformCodes.size());
    });

    return temp;
}

long double BMP::byteSizeCodeShannonFano()
{
    using namespace std;
    long double temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&temp](bitMap::GreyCount a){
        temp += (a.frq * a.codeShannonFano.size());
    });

    return temp;
}

long double BMP::byteSizeCodeHafman()
{
    using namespace std;
    long double temp = 0;

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&temp](bitMap::GreyCount a){
        temp += (a.frq * a.codeHafman.size());
    });

    return temp;
}

unsigned short BMP::read_u16(FILE *fp)
{
    unsigned char b0, b1;

    b0 = (unsigned char)getc(fp);
    b1 = (unsigned char)getc(fp);

    return ((b1 << 8) | b0);
}

unsigned int BMP::read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = (unsigned char)getc(fp);
    b1 = (unsigned char)getc(fp);
    b2 = (unsigned char)getc(fp);
    b3 = (unsigned char)getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

int BMP::read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = (unsigned char)getc(fp);
    b1 = (unsigned char)getc(fp);
    b2 = (unsigned char)getc(fp);
    b3 = (unsigned char)getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}
