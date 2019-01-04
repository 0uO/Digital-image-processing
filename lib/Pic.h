#include<iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "matrix.h"
#include "gdal\gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

bool assert(const bool f, const char * s = "\0");

bool existFile(const char *_path);

bool in(const int x,const int l,const int r);


class Pixel{
public:
	float r,g,b;
	Pixel(const float _r=0, const float _g=0, const float _b=0);
	Pixel(const Pixel &p);

	bool set(const unsigned short channal, const float n);
	bool set(const Pixel *p);
	bool set(const Pixel p);

	float getR()const ;
	float getG()const ;
	float getB()const ;
	float getBand(const unsigned short channal)const ;
	
	bool cutOff();
	
	bool add(const Pixel *p) ;
	bool add(const Pixel p) ;

	Pixel plus(const Pixel *p)const;

	Pixel muti(float f)const;

	void print()const ;
};


class Kernel{
public:
	unsigned short Xlen,Ylen;
	float **M;
	
	Kernel(unsigned short _Xlen=1, unsigned short _Ylen=1, float **_M=nullptr);

	Kernel(vector<vector<float> > _M);

	bool set(float **_M = nullptr);

	unsigned short getXlen()const;
	unsigned short getYlen()const;
	float getScale(const short x,const short y)const;

	~Kernel();
};


class metaPicture{
public:
	GDALDataset *poSrcDS, *duplication;
    GByte *buffTmp;
	Pixel ***M; // [X][Y]

	char path[256];
    unsigned short bandNum, MXSize,MYSize;
    unsigned int imgXlen, imgYlen;

	bool ready;

	metaPicture();

	unsigned int getXlen()const;
	unsigned int getYlen()const;
	unsigned short getBandNum()const;
	Pixel ***getPic()const;
	Pixel getPixel(const int x, const int y)const;

	bool update();

	~metaPicture();
};


class Picture:public metaPicture{
public:
	Picture(const char *_path = nullptr);
	Picture(const unsigned int x, const unsigned int y, const unsigned short b = 3);

	bool save(const char* dstPath, const char* format="JPEG");

	bool Cov(Kernel *k);

	~Picture();
};


Picture* IHS(Picture* P_MUL, Picture* P_PAN);


class LargePicture: public metaPicture {
public:
	unsigned int pointerX,pointerY;
	unsigned short blockXlen,blockYlen;

	LargePicture(const char *_path, const unsigned short B_X = 0, const unsigned short B_Y = 256,const char *dst_path = "\0");
	LargePicture(const unsigned int x, const unsigned int y, const unsigned short b = 3, const unsigned short B_X = 0, const unsigned short B_Y = 256, const char *dst_path = "\0");
	
	unsigned short getBlockXlen()const;
	unsigned short getBlockYlen()const;
	unsigned int getXpointer()const;
	unsigned int getYpointer()const;

	bool nextPointer();
	unsigned short nextLoadSizeX();
	unsigned short nextLoadSizeY();
	bool next();
	void reset();
	
	bool update();

	~LargePicture();
};


LargePicture* IHS(LargePicture* P_MUL, LargePicture* P_PAN, const char* dst_file = "ans.tif");
