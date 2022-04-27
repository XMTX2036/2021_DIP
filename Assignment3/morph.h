#ifndef __MORPH_H__
#define __MORPH_H__

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

int Ostu(BYTE *graydata, double *variance);
void erode(BYTE *graydata, BYTE *erosion, int size, int struct_size);
void dialate(BYTE *graydata, BYTE *dialation, int size, int struct_size);
void opening(BYTE *graydata, BYTE *opening_data, int size, int struct_size);
void closing(BYTE *graydata, BYTE *closing_data, int size, int struct_size);

#endif