#include <iostream>
#include "files.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");
    BMP file( (char *)"D:\\Git\\INF\\src8.bmp" );
    ofstream out("D:\\Git\\INF\\output.txt");
    file.writeInfo();
    file.writeMatrix();
    file.writeFrequency();
    cout << "\nEntropy: " << file.getEntropy() << endl;
    file.writeCodesOfMessage(HUFFMAN);
    file.writeCodesOfMessage(UNIFORM);
    file.writeCodesOfMessage(SHANNON);
    file.writeCodesOfSymbols(HUFFMAN);
    file.writeCodesOfSymbols(UNIFORM);
    file.writeCodesOfSymbols(SHANNON);
    cout << "Size Uniform Codes: " << file.byteSizeCode(UNIFORM)<< endl;
    cout << "Size Code Shannon:  " << file.byteSizeCode(SHANNON)<< endl;
    cout << "Size Code Huffman:  " << file.byteSizeCode(HUFFMAN)<< endl << endl;
	cout << "Compression ratio Shannon's code: " << file.byteSizeCode(UNIFORM) / file.byteSizeCode(SHANNON) << endl;
	cout << "Compression ratio Huffman's code: " << file.byteSizeCode(UNIFORM) / file.byteSizeCode(HUFFMAN) << endl;
    long double shannonSize, huffmanSize, uniformSize;
    uniformSize = file.sizeCode(UNIFORM);
    shannonSize = file.sizeCode(SHANNON);
    huffmanSize = file.sizeCode(HUFFMAN);
    cout << uniformSize << ' ' << shannonSize << ' ' << huffmanSize << '\n';
    cout << "Redundancy Shannon's code: " << (uniformSize-shannonSize)/shannonSize << endl;
    cout << "Redundancy Huffman's code: " << (uniformSize-huffmanSize)/huffmanSize << endl;
    out.close();
    //system("pause>>null");
    return 0;
}