#include <iostream>
#include "files.h"
using namespace std;

int main()
{
    BMP file( (char *)"D:\\Git\\INF\\src.bmp" );
    file.writeInfo();
    file.writeMatrix();

    system("pause>>null");
    return 0;
}

