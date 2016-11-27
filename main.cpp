#include <iostream>
#include "files.h"

using namespace std;

int main()
{
    BMP file( (char *)"D:\\Git\\INF\\src.bmp" );
    ofstream out("output.txt");
    file.writeInfo();
    //file.writeMatrix();
    file.writeFrequency();
    cout << "\nEntropy: " << file.getEntropy() << endl;
    cout << "Minimal Binary Code: " << file.getminBinaryCode() << endl;
    //file.getUniformCodes(out);
    //file.getCodeShannonFano(out);
    file.getCodeHafman(out);
    cout << "Size Uniform Codes: " << file.byteSizeCodeUniform() << endl;
    cout << "Size Code Shennon:  " << file.byteSizeCodeShannonFano() << endl;
    cout << "Size Code Huffman:   " << file.byteSizeCodeHafman() << endl << endl;
	cout << "Compression ratio Shennon's code: " << file.byteSizeCodeUniform() / file.byteSizeCodeShannonFano() << endl;
	cout << "Compression ratio Huffman's code: " << file.byteSizeCodeUniform() / file.byteSizeCodeHafman() << endl;

    long double shennonSize, huffmanSize, uniformSize;
    uniformSize = file.sizeCodeUniform();
    shennonSize = file.sizeCodeShannonFano();
    huffmanSize = file.sizeCodeHafman();
    cout << "Redundancy Shennon's code: " << (uniformSize-shennonSize)/shennonSize << endl;
    cout << "Redundancy Huffman's code: " << (uniformSize-huffmanSize)/huffmanSize << endl;

    //system("pause>>null");
    return 0;
}