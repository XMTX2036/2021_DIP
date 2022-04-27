#include <iostream>
#include <cstdio>
#include <string>
#include "bmp.h"
using namespace std;

WORD bfType;
BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;


int main(int argc, char *argv[])
{
    string bmpname;
    cout << "Please enter the file name of which you want to deal with: ";
    cin >> bmpname;
    FILE *fpread;
    fpread = fopen((bmpname+".bmp").c_str(), "rb");
    if(fpread){
        fread(&bfType, 1, sizeof(WORD), fpread);
        if(bfType != 0x4d42){
            cout << "Wrong Type of File!" << endl;
            return 0;
        }
		fread(&bmpFile, 1, sizeof(BITMAPFILEHEADER), fpread);
		fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fpread);
        int size = bmpInfo.biSizeImage/3;

        RGB *bmpdata = new RGB[size];
        RGB *res = new RGB[size];
        fread(bmpdata, 1, sizeof(RGB)*size, fpread);
        fclose(fpread);
        int width = bmpInfo.biWidth, height = bmpInfo.biHeight;
        width = (width*bmpInfo.biBitCount/8+3)/4*4;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                if((i==0||i==height-1) || (j==0||j==width-1)) res[i*width+j] = bmpdata[i*width+j];
                else{
                    res[i*width+j].B = (bmpdata[i*width+j+1].B+bmpdata[i*width+j].B+bmpdata[i*width+j+1].B
                    +bmpdata[(i-1)*width+j+1].B+bmpdata[(i-1)*width+j].B+bmpdata[(i-1)*width+j+1].B
                    +bmpdata[(i+1)*width+j+1].B+bmpdata[(i+1)*width+j].B+bmpdata[(i+1)*width+j+1].B) / 9;

                    res[i*width+j].R = (bmpdata[i*width+j+1].R+bmpdata[i*width+j].R+bmpdata[i*width+j+1].R
                    +bmpdata[(i-1)*width+j+1].R+bmpdata[(i-1)*width+j].R+bmpdata[(i-1)*width+j+1].R
                    +bmpdata[(i+1)*width+j+1].R+bmpdata[(i+1)*width+j].R+bmpdata[(i+1)*width+j+1].R) / 9;

                    res[i*width+j].G = (bmpdata[i*width+j+1].G+bmpdata[i*width+j].G+bmpdata[i*width+j+1].G
                    +bmpdata[(i-1)*width+j+1].G+bmpdata[(i-1)*width+j].G+bmpdata[(i-1)*width+j+1].G
                    +bmpdata[(i+1)*width+j+1].G+bmpdata[(i+1)*width+j].G+bmpdata[(i+1)*width+j+1].G) / 9;
                    res[i*width+j].B = overflow(res[i*width+j].B);
                    res[i*width+j].G = overflow(res[i*width+j].G);
                    res[i*width+j].R = overflow(res[i*width+j].R);
                }
            }
        }
        FILE *fpwrite;
        fpwrite = fopen((bmpname+"_mean_filter.bmp").c_str(), "wb");
        fwrite(&bfType, 1, sizeof(WORD), fpwrite);
        fwrite(&bmpFile, 1, sizeof(BITMAPFILEHEADER), fpwrite);
        fwrite(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fpwrite);
        fwrite(res, 1, sizeof(RGB)*size, fpwrite);
        fclose(fpwrite);
        cout << bmpInfo.biWidth*bmpInfo.biHeight << endl;
        cout << size << endl;
        int w[3];
        double c;
        cout << "Please enter the coefficient of Laplace transform: ";
        cin >> c;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                if((i==0||i==height-1) || (j==0||j==width-1)) res[i*width+j] = bmpdata[i*width+j];
                else{
                    w[0] = bmpdata[(i-1)*width+j].B+bmpdata[(i+1)*width+j].B
                    +bmpdata[i*width+j+1].B+bmpdata[i*width+j-1].B-4*bmpdata[i*width+j].B;
                    w[1] = bmpdata[(i-1)*width+j].G+bmpdata[(i+1)*width+j].G
                    +bmpdata[i*width+j+1].G+bmpdata[i*width+j-1].G-4*bmpdata[i*width+j].G;
                    w[2] = bmpdata[(i-1)*width+j].R+bmpdata[(i+1)*width+j].R
                    +bmpdata[i*width+j+1].R+bmpdata[i*width+j-1].R-4*bmpdata[i*width+j].R;
                    for(int i=0; i<3; i++){
                        if(w[i] > 0) w[i] = overflow(abs(w[i]));
                    }
                    res[i*width+j].B = bmpdata[i*width+j].B + c*w[0];

                    res[i*width+j].G = bmpdata[i*width+j].G + c*w[1];

                    res[i*width+j].R = bmpdata[i*width+j].R + c*w[2];
                }
            }
        }
        fpwrite = fopen((bmpname+"_Laplace.bmp").c_str(), "wb");
        fwrite(&bfType, 1, sizeof(WORD), fpwrite);
        fwrite(&bmpFile, 1, sizeof(BITMAPFILEHEADER), fpwrite);
        fwrite(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fpwrite);
        fwrite(res, 1, sizeof(RGB)*size, fpwrite);
        fclose(fpwrite);
    }else cout << "Fail to open the file." << endl;
    return 0;
}