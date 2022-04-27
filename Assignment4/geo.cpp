#include "bmp.h"
#include "geo.h"
#include <cmath>

extern WORD bfType;
extern BITMAPFILEHEADER bmpFile;
extern BITMAPFILEHEADER newFile;
extern BITMAPINFOHEADER bmpInfo;
extern BITMAPINFOHEADER newInfo;

void write(string name, RGB *data, int size, bool flag)
{
	FILE *fp;
	fp = fopen((name+".bmp").c_str(), "wb");
	if(flag){
		fwrite(&bfType, 1, sizeof(WORD), fp);
		fwrite(&newFile, 1, sizeof(BITMAPFILEHEADER), fp);
		fwrite(&newInfo, 1, sizeof(BITMAPINFOHEADER), fp);
		fwrite(data, 1, sizeof(RGB)*size, fp);		
	}else{
		fwrite(&bfType, 1, sizeof(WORD), fp);
		fwrite(&bmpFile, 1, sizeof(BITMAPFILEHEADER), fp);
		fwrite(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fp);
		fwrite(data, 1, sizeof(RGB)*size, fp);
	}
	fclose(fp);
}

void mirror(RGB* origin)
{
	int size = bmpInfo.biSizeImage/3;
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;

	RGB *res = new RGB[size];
	memset(res, 0, sizeof(RGB)*size);
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++) res[i*width+j] = origin[(i+1)*width-1-j];	
	}
    string name = "mirror";
    write(name, res, bmpInfo.biSizeImage/3, false);

	delete[] res;
}

void translation(RGB *origin, int dx, int dy)
{
	int size = bmpInfo.biSizeImage/3;
	// make the deltaX can be divided by 4 perfectly
	int deltaX = (dx%4)?(dx/4*4):dx;
	int deltaY = dy;
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;

	int newHeight = height + deltaY;
	int newWidth = width + deltaX;
	int newSize = newHeight * newWidth;
	RGB *res = new RGB[newSize];
	// set the whole new picture white
	for(int i=0; i<newSize; i++) res[i].B = res[i].G = res[i].R = 255;
	
	for(int i=0; i<newHeight; i++){
		for(int j=0; j<newWidth; j++)
			if(i>=deltaY && j>=deltaX) res[i*newWidth+j] = origin[(i-deltaY)*width+(j-deltaX)];
	}
	newFile = bmpFile;
	newInfo = bmpInfo;
	// new size should be updated
	newFile.bfSize += (newSize*3-size*3);
	newInfo.biHeight = newHeight;
	newInfo.biWidth = newWidth;
	newInfo.biSizeImage = newSize*3;
    string name = "translation";
    write(name, res, newSize, true);

	delete[] res;
}

Point rotate(Point p, double theta)
{
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;
	int cx = width/2;
	int cy = height/2;
	p.x = p.x - cx;
	p.y = p.y - cy;
	double newX = p.x * cos(theta) - p.y * sin(theta) + cx;   
    double newY = p.x * sin(theta) + p.y * cos(theta) + cy;
    Point res = {newX, newY};
    return res;
}

// find the boundary of new image after rotation 
// -> range of x and y axis
void FindBoundary(int &x, int &y, double theta)
{
	double newX[4], newY[4];
	double minX = INF, minY = INF, maxX = 0, maxY = 0;
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;
	Point p[4] = {{0, 0}, {width, 0}, {width, height}, {0, height}};
	for(int i=0; i<4; i++){
        Point ans = rotate(p[i], theta);
        newX[i] = ans.x;
        newY[i] = ans.y;
	}
 
	sort(newX, newX+4);
	sort(newY, newY+4);
	x = (int)(newX[3] - newX[0]);
	y = (int)(newY[3] - newY[0]);
	if(x % 4) x = (x/4+1)*4;
}
void rotation(RGB* origin, double theta)
{
	int size = bmpInfo.biSizeImage/3;
	int newHeight;
	int newWidth;
	FindBoundary(newWidth, newHeight, theta);
	int newSize = newHeight * newWidth;
	RGB *res = new RGB[newSize];
	bool *colored = new bool[newSize];
	int *leftBoundary = new int[newHeight];
	int *rightBoundary = new int[newHeight];
 
	for(int i=0; i<newSize; i++) res[i].B = res[i].G = res[i].R = 255;
	for(int i=0; i<newHeight; i++) leftBoundary[i] = INF;
	memset(rightBoundary, 0, sizeof(int)*newHeight);

	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;
	for(int i=0; i<newSize; i++) colored[i] = true;
	for(int i=0; i < height; i++){
		for(int j=0; j<width; j++){
			Point temp = {j, i}, ans;
            ans = rotate(temp, theta);
            int nx = (int)ans.x + (newWidth-width)/2;
			int ny = (int)ans.y + (newHeight-height)/2;
			res[ny*newWidth+nx] = origin[i*width+j];
			colored[ny*newWidth+nx] = false;
			leftBoundary[ny] = min(nx, leftBoundary[ny]);
			rightBoundary[ny] = max(nx, rightBoundary[ny]);
		}
	}
	// 插值
	for(int i=0; i<newHeight; i++){
		for(int j=0; j<newWidth; j++){
			if(j>=leftBoundary[i] && j<=rightBoundary[i]){
				if(colored[i*newWidth+j]){
					int k = j;
					while(k<newWidth && colored[i*newWidth+k]) k++;
					res[i*newWidth+j] = res[i*newWidth+k];
					colored[i*newWidth+j] = false;
				}
			}
		}
	}

	newFile = bmpFile;
	newInfo = bmpInfo;
	newFile.bfSize = newFile.bfSize-size*3+newSize*3;
	newInfo.biHeight = newHeight;
	newInfo.biWidth = newWidth;
	newInfo.biSizeImage = newSize*3;
	string name = "rotation";
	write(name, res, newSize, true);
	delete[] res;
	delete[] colored;
}

void shear(RGB *origin, double angle)
{
	int size = bmpInfo.biSizeImage/3;
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;
	int newWidth = width;
	int newHeight = bmpInfo.biHeight + tan(angle)*width;
	int newSize = newWidth * newHeight;
	RGB *res = new RGB[newSize];
	for(int i=0; i<newSize; i++) res[i].B = res[i].G = res[i].R = 255;
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			int newY = floor(i+tan(angle)*j);
			res[newY*newWidth+j] = origin[i*width+j];
		}
	}
	newFile = bmpFile;
    newInfo = bmpInfo;
	newFile.bfSize = newFile.bfSize - size*3 + newSize*3;
	newInfo.biHeight = newHeight;
	newInfo.biWidth = newWidth;
	newInfo.biSizeImage = newSize*3;
	string name = "shear";
	write(name, res, newSize, true);
    
	delete[] res;
}

void scale(RGB *origin, double Xrate, double Yrate)
{
	int size = bmpInfo.biSizeImage/3;
	int height = bmpInfo.biHeight;
	int width = bmpInfo.biWidth;
	width = (width%4)?(width/4+1)*4:width;

	int newHeight = (int)(bmpInfo.biHeight * Yrate);
	int newWidth = (int)(bmpInfo.biWidth * Xrate);
	newWidth = (newWidth%4)?((newWidth/4+1)*4):newWidth;

	int newSize = newHeight * newWidth;
	RGB *res = new RGB[newSize];
	// set the whole new image as white background
	for(int i=0; i<newSize; i++) res[i].B = res[i].G = res[i].R = 255;
	
	for(int i=0; i<newHeight; i++){
		for(int j=0; j<newWidth; j++){
			int newX = (int)(j*1.0/Xrate);
			int newY = (int)(i*1.0/Yrate);
			res[i*newWidth+j] = origin[newY*width+newX];
		}
	}
    
	newFile = bmpFile;
	newInfo = bmpInfo;
	newFile.bfSize = newFile.bfSize-size*3+newSize*3;
	newInfo.biHeight = newHeight;
	newInfo.biWidth = newWidth;
	newInfo.biSizeImage = newSize*3;
	string name = "scale";
	write(name, res, newSize, true);

	delete[] res;
}