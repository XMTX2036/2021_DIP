#include "morph.h"
#include "bmpio.h"
#include <iostream>
#include <algorithm>
#include <valarray>
#include <cstdio>
using namespace std;

extern WORD bfType;
extern BITMAPFILEHEADER bmpFile;
extern BITMAPINFOHEADER bmpInfo;

int Ostu(BYTE *graydata, double *variance)
{
	int res, size = bmpInfo.biSizeImage;
    int max_value = *max_element(graydata, graydata+size);
    int min_value = *min_element(graydata, graydata+size);
    for(int L=min_value+1; L<=max_value; L++){
        int cnt_fgrd = 0, cnt_bgrd = 0;
        double mean_fgrd = 0, mean_bgrd = 0, p_fgrd, p_bgrd;        
        for(int i=0; i<size; i++){
            if((int)graydata[i] < L){
                cnt_fgrd++;
                mean_fgrd += graydata[i];
            }else{
                cnt_bgrd++;
                mean_bgrd += graydata[i];
            }
        }
        // cout << cnt_fgrd << ";"<< cnt_bgrd << endl;
        mean_fgrd /= (1.0*cnt_fgrd);
        mean_bgrd /= (1.0*cnt_bgrd);
        // if(L<10) cout << "mean value: " << mean_fgrd << ";" << mean_bgrd << endl;
        p_bgrd = cnt_bgrd*1.0 / size;
        p_fgrd = cnt_fgrd*1.0 / size;
        variance[L] = p_fgrd*p_bgrd*(mean_bgrd-mean_fgrd)*(mean_bgrd-mean_fgrd);
        // cout << "probability" << p_fgrd << ";" << p_bgrd << endl;
        // cout << distance(variance, max_element(variance, variance+256)) << endl;
    }
    res = distance(variance, max_element(variance, variance+256));
	return res;
}

void erode(BYTE *graydata, BYTE *erosion, int size, int struct_size)
{
	int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int lineByte = (width*bmpInfo.biBitCount / 8 + 3) / 4 * 4;

	for(int i=struct_size/2; i<height-struct_size/2; i++){
		for(int j=struct_size/2; j<lineByte-struct_size/2; j++){
			bool flag = true;
			for(int k=-struct_size/2; k<=struct_size/2; k++){
				for(int l=-struct_size/2; l<=struct_size/2; l++){
					if(!graydata[(i+k)*lineByte+j+3*l]) flag = false;
				}
			}
			if(flag) erosion[i*lineByte+j] = 255;
			else erosion[i*lineByte+j] = 0;
		}
	}	
}

void dialate(BYTE *graydata, BYTE *dialation, int size, int struct_size)
{
	int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int lineByte = (width*bmpInfo.biBitCount / 8 + 3) / 4 * 4;

	for(int i=struct_size/2; i<height-struct_size/2; i++){
		for(int j=struct_size/2; j<lineByte-struct_size/2; j++){
			bool flag = false;
			for(int k=-struct_size/2; k<=struct_size/2; k++){
				for(int l=-struct_size/2; l<=struct_size/2; l++){
					if(graydata[(i+k)*lineByte+j+3*l] == 255) flag = true;
				}
			}
			if(flag) dialation[i*lineByte+j] = 255;
			else dialation[i*lineByte+j] = 0;
		}
	}    
}
void opening(BYTE *graydata, BYTE *opening_data, int size, int struct_size)
{
    BYTE *temp = new BYTE[size];
    erode(graydata, temp, size, struct_size);
    dialate(temp, opening_data, size, struct_size);
    delete[] temp;
}
void closing(BYTE *graydata, BYTE *closing_data, int size, int struct_size)
{
    BYTE *temp = new BYTE[size];
    dialate(graydata, temp, size, struct_size);
    erode(temp, closing_data, size, struct_size);
    delete[] temp;
}