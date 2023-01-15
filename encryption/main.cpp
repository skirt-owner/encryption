//
//  main.cpp
//  encryption
//
//  Created by skirt owner on 11.01.2023.
//

#include <stdio.h>

const int BYTES_PER_PIXEL = 3;
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

unsigned char* createBitmapFileHeader(int height, int bytes_width_with_padding);
unsigned char* createBitmapInfoHeader(int height, int width);
void generateBitmapImage(char* imageFileName, int height, int width);

int main(int argc, const char * argv[])
{
    char* imageFileName = (char*) "image.bmp";
    int height = 150;
    int width = 300;
    
    generateBitmapImage(imageFileName, height, width);
    
    return 0;
}

unsigned char* createBitmapFileHeader(int height, int widthByBytesWithPadding)
{
    int fileSize = FILE_HEADER_SIZE * INFO_HEADER_SIZE + height * widthByBytesWithPadding;
    
    static unsigned char bitmapFileHeader[] = {
        0, 0, // file type (BM) // 0-1
        0, 0, 0, 0, // file size in bytes // 2-5
        0, 0, 0, 0, // reserved butes (not used) // 6-9
        0, 0, 0, 0 // offset of the byte where the bitmap pixel array can be found // 10-13
    };
    
    bitmapFileHeader[0] = (unsigned char)(0x42);
    bitmapFileHeader[1] = (unsigned char)(0x4D);
    
    bitmapFileHeader[2] = (unsigned char)(fileSize);
    bitmapFileHeader[3] = (unsigned char)(fileSize >> 8);
    bitmapFileHeader[4] = (unsigned char)(fileSize >> 16);
    bitmapFileHeader[5] = (unsigned char)(fileSize >> 24);
    
    bitmapFileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);
    
    return bitmapFileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width)
{
    static unsigned char bitmapInfoHeader[] = {
        0, 0, 0, 0, // size of the header // 0-3
        0, 0, 0, 0, // bitmap width in pixels // 4-7
        0, 0, 0, 0, // bitmap height in pixels // 8-11
        0, 0, // number of planes // 12-13
        0, 0, // number of bits per pixel (color depth) // 14-15
        0, 0, 0, 0, // compression method // 16-19
        0, 0, 0, 0, // image size // 20-23
        0, 0, 0, 0, // horizontal resolution // 24-27
        0, 0, 0, 0, // vertical resolution // 28-31
        0, 0, 0, 0, // number of colors in the color palette // 32-35
        0, 0, 0, 0 // number of important colors // 36-39
    };
    
    bitmapInfoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
    
    bitmapInfoHeader[4] = (unsigned char)(width);
    bitmapInfoHeader[5] = (unsigned char)(width >> 8);
    bitmapInfoHeader[6] = (unsigned char)(width >> 16);
    bitmapInfoHeader[7] = (unsigned char)(width >> 24);
    
    bitmapInfoHeader[8] = (unsigned char)(height);
    bitmapInfoHeader[9] = (unsigned char)(height >> 8);
    bitmapInfoHeader[10] = (unsigned char)(height >> 16);
    bitmapInfoHeader[11] = (unsigned char)(height >> 24);
    
    bitmapInfoHeader[12] = (unsigned char)(1);
    
    bitmapInfoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);
    
    return bitmapInfoHeader;
}

void generateBitmapImage(char* imageFileName, int height, int width)
{
    FILE* imageFile = fopen(imageFileName, "wb");
    
    int widthByBytes = width * BYTES_PER_PIXEL;
    unsigned char padding[] = {0, 0, 0};
    int paddingSize = (4 - (widthByBytes) % 4) % 4;
    int widthByBytesWithPadding = (widthByBytes) + paddingSize;
    
    unsigned char* fileHeader = createBitmapFileHeader(height, widthByBytesWithPadding);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);
    
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixel[3] = {
                (unsigned char) ((i+j)*255/(height+width)),
                (unsigned char) (j*255/width),
                (unsigned char) (i*255/height)
            };
            fwrite(pixel, 1, BYTES_PER_PIXEL, imageFile);
        }
        fwrite(padding, 1, paddingSize, imageFile);
    }
    fclose(imageFile);
}

/*
 A 24-bit bitmap with Width=1, would have 3 by tes of data per row (blue, green, red) and 1 byte of padding, while Width=2 would have 6 bytes of data and 2 bytes of padding, Width=3 would have 9 bytes of data and 3 bytes of padding, and Width=4 would have 12 bytes of data and no padding.
 */
