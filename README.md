Class for Parse BMP and bulding code of Huffman, Shennon and Uniform code. 
=======

**Metods of class:**
----------------


```c++
//This method creates BMP class from fileName
BMP( char * fileName )

//This method writes value of pixel of image
void writeMatrix( std::ofstream *out = nullptr ); 

//This method writes Infomarion of Header of image
void writeInfo( std::ofstream *out = nullptr );   

//This method write shades of image and their frequency of occurrence (considering quantization X=round(X/20)*20)
void writeFrequency( std::ofstream *out = nullptr ); 

//This method write shades of image and their probability frequency of occurrence (considering quantization 
void writeFrequencyProbability ( std::ofstream *out = nullptr );    
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

//The average length of code
long double     byteSizeCode ( typeOfCode type );

//Length of the message in the code
long int        sizeCode ( typeOfCode type );

//Write Codes Of Message
void            writeCodesOfMessage ( typeOfCode type, std::ofstream *out = nullptr );

//Write Codes Of Symbols
void            writeCodesOfSymbols ( typeOfCode type, std::ofstream *out = nullptr );
```

```c++
//We can use ENUM for access to various types of codes
enum typeOfCode{
    UNIFORM,     //uniform code
    HUFFMAN,     //Huffman's code 
    SHANNON      //Code of Shannon-Fano
};
```
