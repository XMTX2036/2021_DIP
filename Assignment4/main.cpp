#include <iostream>
#include <cmath>
#include <algorithm>
#include "bmp.h"
#include "geo.h"
using namespace std;

WORD bfType;
BITMAPFILEHEADER bmpFile, newFile;
BITMAPINFOHEADER bmpInfo, newInfo;
 
int main(int argc, char *argv[])
{
	FILE *fpread;
	fpread = fopen("5.bmp", "rb");
	if(fpread){
		fread(&bfType, 1, sizeof(WORD), fpread);
		if(bfType != 0x4d42){
			printf("Wrong Type of File!\n");
			return 0;
		}
		fread(&bmpFile, 1, sizeof(BITMAPFILEHEADER), fpread);
		fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fpread);
	    int size = bmpInfo.biSizeImage/3;
        RGB *bmpdata = new RGB[size];
		fread(bmpdata, 1, sizeof(RGB)*size, fpread);
		fclose(fpread);
		int x, y;
		double rateX, rateY, alpha;
		printf("Please enter the deltaX, deltaY for translation: ");
		scanf("%d %d", &x, &y);
		translation(bmpdata, x, y);
		mirror(bmpdata);
		printf("Please enter scale rate for both x and y: ");
		scanf("%lf %lf",  &rateX, &rateY);
		scale(bmpdata, rateX, rateY);
		printf("Please enter the angle value of rotation: ");
		scanf("%lf", &alpha);
		rotation(bmpdata, alpha/180 * PI);
		printf("Please enter the angle value of shear: ");
		scanf("%lf", &alpha);
		shear(bmpdata, alpha/180 * PI);
		delete[] bmpdata;
	}else printf("fail to open the file");
	return 0;
}
