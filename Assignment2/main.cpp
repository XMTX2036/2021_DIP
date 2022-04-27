#include <iostream>
#include <string>
#include <fstream>
#include "bmpio.h"
#include "morph.h"
using namespace std;

WORD bfType;
BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;

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

	int struct_size;
	cout << "Please enter the structure element size: ";
	cin >> struct_size;

	fpwrite = fopen((bmpname+"_erosion.bmp").c_str(), "wb");
	BYTE *erosion_data = new BYTE[size];
	erode(graydata, erosion_data, size, struct_size);
	write(erosion_data, &fpwrite);
	fclose(fpwrite);

	fpwrite = fopen((bmpname+"_dialation.bmp").c_str(), "wb");
	BYTE *dialation_data = new BYTE[size];
	dialate(graydata, dialation_data, size, struct_size);
	write(dialation_data, &fpwrite);
	fclose(fpwrite);

	fpwrite = fopen((bmpname+"_opening.bmp").c_str(), "wb");
	BYTE *opening_data = new BYTE[size];
	opening(graydata, opening_data, size, struct_size);
	write(opening_data, &fpwrite);
	fclose(fpwrite);

	fpwrite = fopen((bmpname+"_closing.bmp").c_str(), "wb");
	BYTE *closing_data = new BYTE[size];
	closing(graydata, closing_data, size, struct_size);
	write(closing_data, &fpwrite);
	fclose(fpwrite);

	delete[] graydata;
	delete[] erosion_data;
	delete[] dialation_data;
    return 0;
}