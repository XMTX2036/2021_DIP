#include "bmpio.h"
#include <iostream>
#include <algorithm>
#include <valarray>
#include <cstdio>
using namespace std;

extern WORD bfType;
extern BITMAPFILEHEADER bmpFile;
extern BITMAPINFOHEADER bmpInfo;


void readHead(FILE **fp)
{
	fread(&bfType, sizeof(WORD), 1, *fp);
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, *fp);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, *fp);	
}
void write(BYTE *data, FILE **fp)
{
	fwrite(&bfType, sizeof(WORD), 1, *fp);
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, *fp);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, *fp);
    fwrite(data, bmpInfo.biSizeImage, 1, *fp);
}
void RGB2gray(BYTE *data, int size)
{
	int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int lineByte = (width*bmpInfo.biBitCount / 8 + 3) / 4 * 4;
    for(int i=0; i<height; i++){
        for(int j=0; j<lineByte-2; j+=3){
			BYTE red, green, blue;
			blue = data[i*lineByte+j];
			green = data[i*lineByte+j+1];
			red = data[i*lineByte+j+2];
            BYTE color = 16 + 0.257*red + 0.504*green + 0.098*blue;
			data[i*lineByte+j] = color;
            data[i*lineByte+j+1] = color;
            data[i*lineByte+j+2] = color;
        }
    }
	bmpInfo.biSizeImage = lineByte*bmpInfo.biHeight;
}