#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "bmpio.h"
#include "morph.h"
using namespace std;

WORD bfType;
BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;

struct RGB{
	BYTE B;
	BYTE G;
	BYTE R;
};

int rgb_num[3][256] = {0};
double rgb_p[3][256];
double rgb_cumulative_p[3][256];
int rgb_equal[3][256];

void equalizeHist1(BYTE graydata[])
{
	int gray_num[256] = {0};
	double gray_p[256];
	double gray_cumulative_p[256];
	int gray_equal[256], size = bmpInfo.biSizeImage;
	for(int i=0; i<size; i++) gray_num[graydata[i]]++;
	for(int i=0; i<256; i++) gray_p[i] = 1.0*gray_num[i]/size;
	gray_cumulative_p[0] = gray_p[0];
	for(int i=1; i<256; i++) gray_cumulative_p[i] = gray_cumulative_p[i-1]+gray_p[i];
	for(int i=0; i<256; i++) gray_equal[i] = floor(255.0 * gray_cumulative_p[i] + 0.5);
	for(int i=0; i<size; i++) graydata[i] = gray_equal[graydata[i]];
}

// void equalizeHist2(RGB bmpdata[])
// {
// 	int size = bmpInfo.biSizeImage/3;
// 	for(int i=0; i<size; i++){
// 		rgb_num[0][bmpdata[i].B]++;
// 		rgb_num[1][bmpdata[i].G]++;
// 		rgb_num[2][bmpdata[i].R]++;
// 	}
// 	for(int i=0; i<256; i++){
// 		rgb_p[0][i] = 1.0*rgb_num[0][i]/size;
// 		rgb_p[1][i] = 1.0*rgb_num[1][i]/size;
// 		rgb_p[2][i] = 1.0*rgb_num[2][i]/size;
// 	}
// 	rgb_cumulative_p[0][0] = rgb_p[0][0];
// 	rgb_cumulative_p[1][0] = rgb_p[1][0];
// 	rgb_cumulative_p[2][0] = rgb_p[2][0];
// 	for(int i=1; i<256; i++){
// 		rgb_cumulative_p[0][i] = rgb_cumulative_p[0][i-1]+rgb_p[0][i];
// 		rgb_cumulative_p[1][i] = rgb_cumulative_p[1][i-1]+rgb_p[1][i];
// 		rgb_cumulative_p[2][i] = rgb_cumulative_p[2][i-1]+rgb_p[2][i];
// 	}
// 	for(int i=0; i<256; i++){
// 		rgb_equal[0][i] = floor(rgb_cumulative_p[0][i]*255 + 0.5);
// 		rgb_equal[1][i] = floor(rgb_cumulative_p[1][i]*255 + 0.5);
// 		rgb_equal[2][i] = floor(rgb_cumulative_p[2][i]*255 + 0.5);
// 	}
// 	for(int i=0; i<size; i++){
// 		rgb_num[0][i] = rgb_equal[0][rgb_num[0][i]];
// 		rgb_num[1][i] = rgb_equal[1][rgb_num[1][i]];
// 		rgb_num[2][i] = rgb_equal[2][rgb_num[2][i]];
// 	}
// }

int main(int argc, char *argv[])
{
	string bmpname;
	cout << "Please enter the bmp file name: ";
	cin >> bmpname;
    FILE *fpread = fopen((bmpname+".bmp").c_str(), "rb");
    FILE *fpwrite = fopen((bmpname+"_gray.bmp").c_str(), "wb");
	readHead(&fpread);
	int size = bmpInfo.biSizeImage;
	BYTE *buffer = new BYTE[size];
    fread(buffer, sizeof(BYTE)*size, 1, fpread);
	RGB2gray(buffer, size);
	write(buffer, &fpwrite);
    fclose(fpwrite);
	fclose(fpread);
    delete[] buffer;

	int threshold;
	double variance[256];
	fpread = fopen((bmpname+"_gray.bmp").c_str(), "rb");
	fpwrite = fopen((bmpname+"_binary.bmp").c_str(), "wb");
	readHead(&fpread);
	size = bmpInfo.biSizeImage;
	BYTE *graydata = new BYTE[size];
	fread(graydata, sizeof(BYTE)*size, 1, fpread);
	fclose(fpread);
	threshold = Ostu(graydata, variance);
	cout << threshold << endl;
	for(int i=0; i<size; i++){
		if(graydata[i]<threshold) graydata[i] = 0;
		else graydata[i] = 255;
	}
	write(graydata, &fpwrite);
	fclose(fpwrite);
    fpread = fopen((bmpname+"_gray.bmp").c_str(), "rb");
    readHead(&fpread);
    size = bmpInfo.biSizeImage;
	fread(graydata, sizeof(BYTE)*size, 1, fpread);
    // for(int i=0; i<10; i++) cout << (int)graydata[i] << " ";
    // cout << endl;
	fclose(fpread);
    fpwrite = fopen((bmpname+"_enhancement.bmp").c_str(), "wb");
    double maxvalue = log(1+*max_element(graydata, graydata+size));
    double minvalue = log(1+*min_element(graydata, graydata+size));
    cout << maxvalue << endl << minvalue << endl;
    for(int i=0; i<size; i++) graydata[i] = (int)(255*(log(1+graydata[i])-minvalue)/(maxvalue-minvalue));
    write(graydata, &fpwrite);
    fclose(fpwrite);    
    // for(int i=0; i<10; i++) cout << (int)graydata[i] << " ";
    // cout << endl;
    fpread = fopen((bmpname+"_gray.bmp").c_str(), "rb");
    readHead(&fpread);
    
	fread(graydata, sizeof(BYTE)*size, 1, fpread);
    // for(int i=0; i<10; i++) cout << (int)graydata[i] << " ";
    // cout << endl;
	fclose(fpread);
	equalizeHist1(graydata);
    fpwrite = fopen((bmpname+"_equal1.bmp").c_str(), "wb");
    write(graydata, &fpwrite);
    fclose(fpwrite);
	// fpread = fopen((bmpname+".bmp").c_str(), "rb");
	// readHead(&fpread);
	// size = bmpInfo.biSizeImage/3;
	// RGB *bmpdata = new RGB[size];
	// fread(bmpdata, sizeof(RGB)*size, 1, fpread);
	// fclose(fpread);
	// fpwrite = fopen((bmpname+"_equal2.bmp").c_str(), "wb");
	// equalizeHist2(bmpdata);
	// fwrite(&bfType, sizeof(WORD), 1, fpwrite);
    // fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, fpwrite);
    // fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fpwrite);
    // fwrite(bmpdata, bmpInfo.biSizeImage/3, 1, fpwrite);
	// fclose(fpwrite);

	delete[] graydata;
    // delete[] bmpdata;
	return 0;
}