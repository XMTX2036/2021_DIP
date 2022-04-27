#ifndef __GEO_H__
#define __GEO_H__

#include "bmp.h"
#include <string>
using namespace std;

#define PI 3.1415962535
#define INF 65535

struct Point{
	int x, y;
};	

void write(string name, RGB *data, int size, bool flag);

void mirror(RGB *origin);

void translation(RGB *origin, int dx, int dy);

Point rotate(Point p, double theta);
void FindBoundary(int &x, int &y, double theta);
void rotation(RGB* origin, double theta);
 
void shear(RGB *origin, double angle);
//缩小放大
void scale(RGB *origin, double Xrate, double Yrate);

#endif