 It's Class for Parse BMP and bulding code of Huffman, Shennon and Uniform code. 
=======

**Metods of class:**
----------------


```c++
//This method creates BMP class from fileName
BMP( char * fileName )

//This method writes value of pixel of image
void writeMatrix(); 

//This method writes Infomarion of Header of image
void writeInfo();   

/*This method write shades of image and their frequency of occurrence in file(considering quantization X=round(X/20)*20)*/
void writeFrequency(); 
```
**These methods return Infomarion of Header:**
----------------
```c++
    unsigned long   getBitSize();
    int             getWidth();
    int             getHeight();
    unsigned short  getBitCount();
    unsigned int    getCompression();
    unsigned int    getSizeImage();
    int             getXPelsPerMeter();
    int             getYPelsPerMeter();
    unsigned int    getClrUsed();
    unsigned int    getClrImportant();
```
**Other methods:**
----------------
```c++
//Value of Entropy
long double     getEntropy();

//The minimum size of the binary code for encoding
int             getminBinaryCode(); 

//Write into file uniform code
void            getUniformCodes( std::ofstream &out );

//Write into file code of Shannon-Fano
void            getCodeShannonFano( std::ofstream &out );

//Write into file code of Huffman
void            getCodeHafman( std::ofstream &out );

//The average length of binary uniform code
long double     byteSizeCodeUniform(); 

//The average length of binary code of Shannon-Fano
long double     byteSizeCodeShannonFano(); 

//The average length of binary code of Huffman
long double     byteSizeCodeHafman();

//Length of the message in the uniform code
long int        sizeCodeUniform();

//Length of the message in the code of Shannon-Fano
long int        sizeCodeShannonFano();

//Length of the message in the code of Huffman
long int        sizeCodeHafman();
```
