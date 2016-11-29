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
    file.writeCodesOfMessage(SHENNON);
    file.writeCodesOfSymbols(HUFFMAN);
    file.writeCodesOfSymbols(UNIFORM);
    file.writeCodesOfSymbols(SHENNON);
    cout << "Size Uniform Codes: " << file.byteSizeCode(UNIFORM)<< endl;
    cout << "Size Code Shennon:  " << file.byteSizeCode(SHENNON)<< endl;
    cout << "Size Code Huffman:  " << file.byteSizeCode(HUFFMAN)<< endl << endl;
	cout << "Compression ratio Shennon's code: " << file.byteSizeCode(UNIFORM) / file.byteSizeCode(SHENNON) << endl;
	cout << "Compression ratio Huffman's code: " << file.byteSizeCode(UNIFORM) / file.byteSizeCode(HUFFMAN) << endl;
    long double shennonSize, huffmanSize, uniformSize;
    uniformSize = file.sizeCode(UNIFORM);
    shennonSize = file.sizeCode(SHENNON);
    huffmanSize = file.sizeCode(HUFFMAN);
    cout << uniformSize << ' ' << shennonSize << ' ' << huffmanSize << '\n';
    cout << "Redundancy Shennon's code: " << (uniformSize-shennonSize)/shennonSize << endl;
    cout << "Redundancy Huffman's code: " << (uniformSize-huffmanSize)/huffmanSize << endl;
    out.close();
    //system("pause>>null");
    return 0;
}