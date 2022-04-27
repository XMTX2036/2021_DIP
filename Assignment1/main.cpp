#include <iostream>
#include <string>
#include "bmp.h"
using namespace std; 

BITMAPFILEHEADER bmpHead;
BITMAPINFOHEADER bmpInfo;
RGBQUAD plate[256];

int main(int argc, char *argv[])
{
	FILE *fp1;
    string bmpname, suffix = ".bmp";
	cout << "Please enter the name of the bmp file(without .bmp) you want to operate: " << endl;
	cin >> bmpname;

	fp1 = fopen((bmpname+suffix).c_str(), "rb");
	if(!fp1) cout << "fail to open " << bmpname << endl;
	else{
		// Here, the value of variable bfType must be read in before seek the fp
        // in the whole file as there is something special about the sizeof().
		WORD bfType;
		fread(&bfType, 1, sizeof(WORD), fp1);
		if(bfType != 0x4D42){
			cout<<"It's not a bmp!"<<endl;
			return 0;
		}

		fread(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp1);
		// cout << bmpInfo.biBitCount << endl;
		if(bmpInfo.biBitCount == 8) fread(plate, sizeof(RGBQUAD), 256, fp1);
		// As the main type of the bmp nowadays is 24 bytes, the colorplate is not essential.
        fseek(fp1, bmpHead.bfOffBits, 0);

        int size = bmpInfo.biSizeImage / 3;
        RGB *bmpdata = new RGB[size];
		fread(bmpdata, 1, sizeof(RGB)*size, fp1);//读取bmp数据信息
		fclose(fp1);

		YUV *res = new YUV[size];
		BYTE red, blue, green;
		for(int i=0; i<size; i++){
            red = bmpdata[i].R;
            blue = bmpdata[i].B;
            green = bmpdata[i].G;
			res[i].Y = 16 + 0.257*red + 0.504*green + 0.098*blue;
			res[i].U = 128 - 0.148*red - 0.291*green + 0.439*blue;
			res[i].V = 128 + 0.439*red - 0.368*green - 0.071*blue;
		}

		// In YUV color space, Y stores nearly all the information of grayscale picture.
		RGB *gray = new RGB[size];
		for(int i=0; i<size; i++){
			gray[i].B = res[i].Y;
			gray[i].G = res[i].Y;
			gray[i].R = res[i].Y;
			// cout << (int)res[i].Y << endl;
		}
		FILE *fp2;
		fp2 = fopen((bmpname+"gray.bmp").c_str(), "wb");
		if(!fp2) cout << "error" << endl;
		fwrite(&bfType, sizeof(WORD), 1, fp2);
		fwrite(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp2);
		fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp2);
		fwrite(gray, sizeof(RGB)*size, 1, fp2);
		fclose(fp2);
		delete[] gray;

		// change the Y
		for(int i=0; i<size; i++) res[i].Y = overflow(res[i].Y + 10);

		FILE *fp3;
		fp3 = fopen((bmpname+"YUV_to_RGB.bmp").c_str(), "wb");
		RGB *recover_rgb = new RGB[size];
		if(!fp3) cout << "wrong" << endl;
		fwrite(&bfType, sizeof(WORD), 1, fp3);
		fwrite(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp3);
		fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp3);
		// There seems to be a lot of standards to transform the RGB to YUV, here I use the
		// standard (BT601) --> 数字YUV <--> 数字RGB 
		for(int i=0;i<size;i++){
            int Y = res[i].Y - 16;
			int U = res[i].U - 128;
			int V = res[i].V - 128;
			// Here I try to use bit operation to simplify and accelerate the performing speed.
			recover_rgb[i].R = overflow((298*Y + 409*V + 128) >> 8);
			recover_rgb[i].G = overflow((298*Y - 100*U - 208*V + 128) >> 8);
			recover_rgb[i].B = overflow((298*Y + 516*U + 128) >> 8);
		}
		fwrite(recover_rgb, sizeof(RGB)*size, 1, fp3);
		fclose(fp3);
		// Here I wrote a part of more complex way to exchange the color bmp into grayscale
		LONG LineByte = (bmpInfo.biWidth*8/8 + 3)/4*4;
		FILE *fp4;
		BYTE *newdata = new BYTE[size];
		fp4 = fopen("another_gray.bmp", "wb");
		fread(&bfType, 1, sizeof(WORD), fp4);
		fread(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp4);
		fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp4);
		fread(newdata, size, 1, fp4);
		if(!fp4) cout << "fail to create" << endl;
		else{
			bmpHead.bfSize = 14+40+sizeof(RGBQUAD)*256+LineByte*bmpInfo.biHeight;
			bmpHead.bfOffBits = 14+40+sizeof(RGBQUAD)*256;
			bmpInfo.biBitCount = 8;
			bmpInfo.biSizeImage = LineByte*bmpInfo.biHeight;
			fwrite(&bfType, sizeof(WORD), 1, fp4);
			fwrite(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp4);
			fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp4);
			for(int i=0; i<256; i++){
				plate[i].rgbRed = i;
				plate[i].rgbBlue = i;
				plate[i].rgbGreen = i;
			}
			fwrite(plate, sizeof(RGBQUAD), 256, fp4);
			int sum = 0, cnt = 0;
			fwrite(newdata, LineByte*bmpInfo.biHeight, 1, fp4);
		}

		fclose(fp4);

		delete[] bmpdata;
		delete[] res;
		delete[] recover_rgb;
	}
	return 0;
}