#include "files.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <map>

bitMap::HuffmanTree::HuffmanTree( HuffmanTree *l, HuffmanTree *r)
{
    left   = l;
    right  = r;
    frq    = l->frq + r->frq;
    posAlb = -1;
}

bitMap::HuffmanTree::HuffmanTree(long double f, int p, HuffmanTree *l, HuffmanTree *r)
{
    frq    = f;
    posAlb = p;
    left   = l;
    right  = r;
}

int BMP::correlation(int a)
{
    return int( std::round ( a / 20 ) * 20 );
}

BMP::BMP( char * fileName )
{
    using namespace bitMap;

    FILE *f = fopen( fileName, "rb" );

    head.bfType      = read_u16( f );
    head.bfSize      = read_u32( f );
    head.bfReserved1 = read_u16( f );
    head.bfReserved2 = read_u16( f );
    head.bfOffBits   = read_u32( f );

    headInfo.biSize          = read_u32( f );
    headInfo.biWidth         = read_s32( f );
    headInfo.biHeight        = read_s32( f );
    headInfo.biPlanes        = read_u16( f );
    headInfo.biBitCount      = read_u16( f );
    headInfo.biCompression   = read_u32( f );
    headInfo.biSizeImage     = read_u32( f );
    headInfo.biXPelsPerMeter = read_s32( f );
    headInfo.biYPelsPerMeter = read_s32( f );
    headInfo.biClrUsed       = read_u32( f );
    headInfo.biClrImportant  = read_u32( f );

    field = new unsigned char* [ headInfo.biWidth ];

    for ( auto i = 0; i < headInfo.biWidth; i++ ){
        field[ i ] = new unsigned char [ headInfo.biHeight ];
    }

    std::fill( shades, shades + 256, 0 );

    int coun = 0;

    for ( auto i = 0; i < headInfo.biWidth; i++ ){
        for ( auto j = 0; j < headInfo.biHeight; j++ ) {
            field[ i ][ j ] = ( unsigned char )getc( f );
            if ( shades[ correlation( field[ i ][ j ] ) ] == 0 ) {
                coun++;
            }

            shades[ correlation( field[ i ][ j ] ) ]++;
        }
        getc( f );
    }

    alphabet         = new Alphabet;
    alphabet->num    = coun;
    alphabet->simbol = new GreyCount [ alphabet->num ];

    coun    = 0;
    int sum = 0;

    for ( auto i = 0; i < 256; i++ ){
        if ( shades[i] != 0 ) {
            alphabet->simbol[ coun ].tone = ( unsigned char )i;
            alphabet->simbol[ coun ].num  = shades[ i ];
            sum += shades[ i ];
            coun++;
        }
    }

    int minBinaryCode = int( std::ceil( ( long double )std::log2( alphabet->num ) ) );

    entropy = 0;

    for ( auto i = 0; i < alphabet->num; i++ ) {
        alphabet->simbol[ i ].frq = ( long double )alphabet->simbol[ i ].num / ( long double )sum;

        entropy = entropy + ( alphabet->simbol[ i ].frq * std::log2( alphabet->simbol[ i ].frq ) );
    }

    entropy *= ( -1 );

    std::sort( alphabet->simbol, alphabet->simbol+alphabet->num, []( GreyCount a, GreyCount b ){ return a.frq < b.frq; } );

    for ( int i = 0; i < alphabet->num; i++ ){
        alphabet->simbol[ i ].uniformCodes = getUmCode( i, minBinaryCode );
    }

    buildingCodeShannonFano( '4', "-", 0, alphabet->num - 1 );
    buildingCodeHuffman();
    fclose(f);
}

void BMP::buildingCodeShannonFano(char lit, std::string code, int left, int right)
{
    if ( lit != '1' && lit != '0' ) {
        code = "";
    } else {
        code += lit;
    }

    if ( left == right ) {
        alphabet->simbol[ left ].codeShannonFano = code;
    } else {
        long double midFrq = 0;
        int medium         = left;
        long double temp   = 0;

        for ( auto i = left; i < right; i++ ){
            midFrq += alphabet->simbol[ i ].frq;
        }

        midFrq /= 2;

        while ( medium < right && temp + alphabet->simbol[ medium ].frq < midFrq){
            temp += alphabet->simbol[ medium ].frq;
            medium++;
        }

        buildingCodeShannonFano( '1', code, left, medium );
        buildingCodeShannonFano( '0', code, medium + 1, right );
    }
}

void BMP::fun( bitMap::HuffmanTree *node, std::string &code)
{
    if ( node->left != nullptr ){
        code += '0';
        fun( node->left, code );
    }

    if ( node->right != nullptr ){
        code += '1';
        fun( node->right, code );
    }

    if ( node->posAlb != -1 ) {
        alphabet->simbol[ node->posAlb ].codeHafman = code;
    }


	if ( !code.empty() ) {
        code.pop_back();
    }
}

void del( bitMap::HuffmanTree *node )
{
    if ( node->left != nullptr ){
        del( node->left );
    }
    if ( node->right != nullptr ){
        del( node->right );
    }

    delete node;
}

void BMP::buildingCodeHuffman()
{
    using namespace bitMap;
    using namespace std;

    auto comp = []( HuffmanTree *a, HuffmanTree*b ){ return a->frq > b->frq; };

    priority_queue< HuffmanTree*, vector< HuffmanTree* >, decltype( comp ) > qu(comp);

    for ( auto i = 0; i < alphabet->num; i++ ){
        HuffmanTree *obj = new HuffmanTree( alphabet->simbol[ i ].frq, i, nullptr, nullptr );
        alphabet->simbol[ i ].codeHafman = "";
        qu.push( obj );
    }


    while( qu.size() > 1 ){
        HuffmanTree *l = qu.top();
        qu.pop();
        HuffmanTree *r = qu.top();
        qu.pop();

        HuffmanTree *root = new HuffmanTree( l ,r );

        qu.push(root);
    }

    HuffmanTree *root = qu.top();

    string code = "";
    fun( root, code );
    del( root );
}

BMP::~BMP()
{
    using namespace bitMap;

    delete[] alphabet->simbol;
    delete[] alphabet;

    for ( int i = 0; i < headInfo.biWidth; i++ ){
        delete[] field[ i ];
    }
    delete[] field;
}

std::string BMP::getUmCode(int a, int n)
{
    using namespace std;

    string x = getBit( a );

    reverse( x.begin(), x.end() );

    while ( x.size() != n )
        x += '0';

    reverse( x.begin(), x.end() );

    return x;
}

std::string BMP::getBit(int a)
{
    using namespace std;

    string x = "";
    while ( a != 0 ){
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

void BMP::writeMatrix(  std::ofstream *out  ) {
    using namespace std;
    bool flag;
    if ( out != nullptr ) {
        flag = true;
    } else {
        flag = false;
    }

    for ( auto i = 0; i < headInfo.biWidth; i++ ) {
        for ( auto j = 0; j < headInfo.biHeight; j++ ) {
            if ( flag ) {
                ( *out ) << setw( 3 ) << setfill( '0' ) << (int)field[ i ][ j ] << ' ';
            } else {
                cout << setw( 3 ) << setfill( '0' ) << (int)field[ i ][ j ] << ' ';
            }
        }

        if (flag) {
            (*out) << endl;
        } else {
            cout << endl;
        }
    }

    if (flag) {
        (*out) << endl;
    } else {
        cout << endl;
    }
}

void BMP::writeCodesOfSymbols(  typeOfCode type, std::ofstream *out ) {
    using namespace std;
    bool flag;
    if ( out != nullptr ) {
        flag = true;
    } else {
        flag = false;
    }

    string title;

    switch( type ){
        case SHENNON:
            title = "Shennon's Code of symbols: \n";
            break;
        case UNIFORM:
            title = "Uniform Code of symbols: \n";
            break;
        case HUFFMAN:
            title = "Huffman's Code of symbols: \n";
            break;
    }

    if ( flag ) {
        (*out) << title;
    } else{
        cout << title;
    }

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [&](bitMap::GreyCount a) {
        switch (type) {
            case SHENNON:
                if (flag) {
                    (*out) << setw(8) << (int) a.tone << ": " << a.codeShannonFano << '\n';
                } else {
                    cout << setw(8) << (int) a.tone << ": " << a.codeShannonFano << '\n';
                }
                break;
            case UNIFORM:
                if (flag) {
                    (*out) << setw(8) << (int) a.tone << ": " << a.uniformCodes << '\n';
                } else {
                    cout << setw(8) << (int) a.tone << ": " << a.uniformCodes << '\n';
                };
                break;
            case HUFFMAN:
                if (flag) {
                    (*out) << setw(8) << (int) a.tone << ": " << a.codeHafman << '\n';
                } else {
                    cout << setw(8) << (int) a.tone << ": " << a.codeHafman << '\n';
                }
                break;
        }
    });
}

void BMP::writeFrequencyProbability( std::ofstream *out )
{
    using namespace std;
    bool flag;
    if ( out != nullptr )
        flag = true;
    else
        flag = false;

    for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( bitMap::GreyCount a ){
        if ( flag ) {
            ( *out ) << setw( 8 ) << fixed << setprecision( 5 ) << ( int ) a.tone << ": " << a.frq << endl;
        } else {
            cout << setw( 8 ) << fixed << setprecision( 5 ) << ( int ) a.tone << ": " << a.frq << endl;
        }
    });
}

void BMP::writeFrequency( std::ofstream *out )
{
    using namespace std;
    bool flag;
    if ( out != nullptr ) {
        flag = true;
    } else {
        flag = false;
    }

    for_each(alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( bitMap::GreyCount a ){
        if ( flag ) {
            (*out) << setw( 8 ) << ( int ) a.tone << ": " << a.num << endl;
        } else {
            cout << setw( 8 ) << ( int ) a.tone << ": " << a.num << endl;
        }
    });
}

void BMP::writeInfo( std::ofstream *out ) {
    using namespace std;

    bool flag;
    if ( out != nullptr ) {
        flag = true;
    } else {
        flag = false;
    }

    if ( flag ) {
        ( *out ) << "General Information of BMP Files\n";
        ( *out ) << "bfType: " << head.bfType << '\n';
        ( *out ) << "bfSize: " << head.bfSize << '\n';
        ( *out ) << "bfReserved1: " << head.bfReserved1 << '\n';
        ( *out ) << "bfReserved2: " << head.bfReserved2 << '\n';
        ( *out ) << "bfOffBits: " << head.bfOffBits << '\n';

        ( *out ) << "\nHeader Information of BMP Files\n";
        ( *out ) << "biSize: " << headInfo.biSize << '\n';
        ( *out ) << "biWidth: " << headInfo.biWidth << '\n';
        ( *out ) << "biHeight: " << headInfo.biHeight << '\n';
        ( *out ) << "biPlanes: " << headInfo.biPlanes << '\n';
        ( *out ) << "biBitCount: " << headInfo.biBitCount << '\n';
        ( *out ) << "biCompression: " << headInfo.biCompression << '\n';
        ( *out ) << "biSizeImage: " << headInfo.biSizeImage << '\n';
        ( *out ) << "biXPelsPerMeter: " << headInfo.biXPelsPerMeter << '\n';
        ( *out ) << "biYPelsPerMeter: " << headInfo.biYPelsPerMeter << '\n';
        ( *out ) << "biClrUsed: " << headInfo.biClrUsed << '\n';
        ( *out ) << "biClrImportant: " << headInfo.biClrImportant << '\n';

    } else {
        cout << "General Information of BMP Files\n";
        cout << "bfType: " << head.bfType << '\n';
        cout << "bfSize: " << head.bfSize << '\n';
        cout << "bfReserved1: " << head.bfReserved1 << '\n';
        cout << "bfReserved2: " << head.bfReserved2 << '\n';
        cout << "bfOffBits: " << head.bfOffBits << '\n';

        cout << "\nHeader Information of BMP Files\n";
        cout << "biSize: " << headInfo.biSize << '\n';
        cout << "biWidth: " << headInfo.biWidth << '\n';
        cout << "biHeight: " << headInfo.biHeight << '\n';
        cout << "biPlanes: " << headInfo.biPlanes << '\n';
        cout << "biBitCount: " << headInfo.biBitCount << '\n';
        cout << "biCompression: " << headInfo.biCompression << '\n';
        cout << "biSizeImage: " << headInfo.biSizeImage << '\n';
        cout << "biXPelsPerMeter: " << headInfo.biXPelsPerMeter << '\n';
        cout << "biYPelsPerMeter: " << headInfo.biYPelsPerMeter << '\n';
        cout << "biClrUsed: " << headInfo.biClrUsed << '\n';
        cout << "biClrImportant: " << headInfo.biClrImportant << '\n';
    }
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

void BMP::writeCodesOfMessage( typeOfCode type, std::ofstream *out ) {
    using namespace std;
    using namespace bitMap;
    map< int, string > dic;

    bool flag;
    if ( out != nullptr ) {
        flag = true;
    } else {
        flag = false;
    }

    string title;
    switch ( type ) {
        case SHENNON:
            title = "Shennon's Code of message: \n";
            for ( auto i = 0; i < alphabet->num; i++ ) {
                dic.insert( make_pair( ( int ) alphabet->simbol[ i ].tone, alphabet->simbol[ i ].codeShannonFano));
            }
            break;
        case UNIFORM:
            title = "Uniform Code of message: \n";
            for ( auto i = 0; i < alphabet->num; i++ ) {
                dic.insert( make_pair( ( int ) alphabet->simbol[ i ].tone, alphabet->simbol[ i ].uniformCodes));
            }
            break;
        case HUFFMAN:
            title = "Huffman's Code of message: \n";
            for ( auto i = 0; i < alphabet->num; i++ ) {
                dic.insert( make_pair( ( int ) alphabet->simbol[ i ].tone, alphabet->simbol[ i ].codeHafman));
            }
            break;
    }

    if ( flag ) {
        ( *out ) << title;
    } else {
        cout << title;
    }

    for ( auto i = 0; i < headInfo.biWidth; i++ ) {
        for ( auto j = 0; j < headInfo.biHeight; j++ ) {
            auto t = correlation( field[ i ][ j ] );
            if ( flag ) {
                ( *out ) << dic[ t ] << ' ';
            } else {
                cout << dic[ t ] << ' ';
            }
        }
    }

    if ( flag ) {
        ( *out ) << "\n\n";
    } else {
        cout << "\n\n";
    }
}

long int BMP::sizeCode( typeOfCode type )
{
    using namespace std;
    using namespace bitMap;
    long int temp = 0;

    switch( type ){
        case SHENNON:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.num * a.codeShannonFano.size() );
            });
            break;
        case UNIFORM:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.num * a.uniformCodes.size() );
            });
            break;
        case HUFFMAN:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.num * a.codeHafman.size() );
            });
            break;
    }

    return temp;
}

long double BMP::byteSizeCode( typeOfCode type )
{
    using namespace std;
    using namespace bitMap;
    long double temp = 0;

    switch( type ){
        case SHENNON:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.frq * a.codeShannonFano.size() );
            });
            break;
        case UNIFORM:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.frq * a.uniformCodes.size() );
            });
            break;
        case HUFFMAN:
            for_each( alphabet->simbol, alphabet->simbol+alphabet->num, [ & ]( GreyCount a ){
                temp += ( a.frq * a.codeHafman.size() );
            });
            break;
    }
    return temp;
}

unsigned short BMP::read_u16( FILE *fp )
{
    unsigned char b0, b1;

    b0 = ( unsigned char )getc( fp );
    b1 = ( unsigned char )getc( fp );

    return ( ( b1 << 8 ) | b0 );
}

unsigned int BMP::read_u32( FILE *fp )
{
    unsigned char b0, b1, b2, b3;

    b0 = ( unsigned char )getc( fp );
    b1 = ( unsigned char )getc( fp );
    b2 = ( unsigned char )getc( fp );
    b3 = ( unsigned char )getc( fp );

    return ( ( ( ( ( ( b3 << 8 ) | b2 ) << 8 ) | b1 ) << 8 ) | b0 );
}

int BMP::read_s32( FILE *fp )
{
    unsigned char b0, b1, b2, b3;

    b0 = ( unsigned char )getc( fp );
    b1 = ( unsigned char )getc( fp );
    b2 = ( unsigned char )getc( fp );
    b3 = ( unsigned char )getc( fp );

    return ( ( int )( ( ( ( ( b3 << 8 ) | b2 ) << 8 ) | b1 ) << 8 ) | b0 );
}
