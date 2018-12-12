#include<iostream>
#include "gdal\gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
#include <fstream>
#include <vector>

using namespace std;

bool assert(const bool f){
	if (!f){
		cout<<"Error occured."<<endl;
		system("PAUSE");
		exit(1);
	}
	return f;
}

class Pixel{
public:
	short r,g,b;
	Pixel(const unsigned short _r=0, const unsigned short _g=0, const unsigned short _b=0):r(_r),g(_g),b(_b){}
	Pixel(const Pixel &p){
		r = p.r;
		g = p.g;
		b = p.b;
	}
	bool set(const unsigned short channal, const unsigned short n){
		if(channal==0){
			r = n;
		} else if(channal==1){
			g = n;
		} else if(channal==2){
			b = n;
		} else {
			return false;
		}
		return true;
	}

	bool set(const Pixel *p){
		r = p->getR();
		g = p->getG();
		b = p->getB();
		return true;
	}
	
	bool set(const Pixel p){
		r = p.getR();
		g = p.getG();
		b = p.getB();
		return true;
	}

	unsigned short getR()const {
		return (unsigned short)r;
	}
	unsigned short getG()const {
		return (unsigned short)g;
	}
	unsigned short getB()const {
		return (unsigned short)b;
	}
	unsigned short getBand(const unsigned short channal)const {
		if(channal==0){
			return getR();
		} else if(channal==1){
			return getG();
		} else if(channal==2){
			return getB();
		} else {
			return 0;
		}
	}
	
	bool cutOff(){
		if(r > 255) r=255;
		if(g > 255) r=255;
		if(b > 255) r=255;
		if(r <0) r=0;
		if(g <0) g=0;
		if(b <0) b=0;
		return true;
	}
	
	bool add(const Pixel *p) {
		r += p->getR();
		g += p->getG();
		b += p->getB();
		return true;
	}	
	bool add(const Pixel p) {
		r += p.getR();
		g += p.getG();
		b += p.getB();
		return true;
	}

	Pixel plus(const Pixel *p)const{
		Pixel tmpP = *this;
		tmpP.r += p->getR();
		tmpP.g += p->getG();
		tmpP.b += p->getB();
		return tmpP;
	}

	Pixel muti(float f)const{
		Pixel tmpP;
		tmpP.r = (short)((float)r*f);
		tmpP.g = (short)((float)g*f);
		tmpP.b = (short)((float)b*f);
		return tmpP;
	}

	void print()const {
		cout<<"r: "<<r<<"    g: "<<g<<"    b: "<<b<<" | ";
	}
};

class Kernel{
public:
	unsigned short Xlen,Ylen;
	float **M;
	
	Kernel(unsigned short _Xlen=1, unsigned short _Ylen=1, float **_M=nullptr){
		assert(_Xlen!=0&&_Ylen!=0);
		assert(
			_M != nullptr &&
			(sizeof(_M) / sizeof(_M[0]) == _Xlen) &&
			(sizeof(_M[0]) / sizeof(_M[0][0]) == _Ylen)
				);
		Xlen = _Xlen;
		Ylen = _Ylen;
		M = new float *[Xlen];
		for (int i = 0;i<Xlen;i++){
			M[i] = new float [Ylen];
			for (int j = 0;j < Ylen ;j++){
				M[i][j] = _M[i][j];
			}
		}
	}

	Kernel(vector<vector<float> > _M){
		Xlen = _M.size();
		Ylen = _M[0].size();
		M = new float *[Xlen];
		for (int i = 0;i<Xlen;i++){
			M[i] = new float [Ylen];
			for (int j = 0;j < Ylen ;j++){
				M[i][j] = _M[i][j];
			}
		}
	}

	bool set(float **_M = nullptr){
		assert(
			_M != nullptr &&
			(sizeof(_M) / sizeof(_M[0]) == Xlen) &&
			(sizeof(_M[0]) / sizeof(_M[0][0]) == Ylen)
				);
		for (int i = 0;i<Xlen;i++){
			for (int j = 0;j < Ylen ;j++){
				M[i][j] = _M[i][j];
			}
		}
		return true;
	}

	unsigned short getXlen()const{
		return Xlen;
	}

	unsigned short getYlen()const{
		return Ylen;
	}

	float getScale(const short x,const short y)const{
		return M[x][y];
	}

	~Kernel(){
		delete []M;
	}
};

class Picture{
public:
	GDALDataset *poSrcDS, *duplication;
	char path[256];
    GByte *buffTmp;
	Pixel ***M;
    unsigned short imgXlen, imgYlen,bandNum;

	bool ready;

	bool existFile(const char *_path){
		if (_path == nullptr)
			return false;
		fstream _file;
		_file.open(_path,ios::in);
		if(!_file)
			return false;
		return true;
	}
	
	bool in(const int x,const int l,const int r)const{
		return x>=l && x<=r;
	}

	Picture(const char *_path = nullptr){
		
		GDALAllRegister();
		ready = false;

		if (existFile(_path)){
			strcpy(path, _path);
			poSrcDS = (GDALDataset*)GDALOpenShared(path,GA_ReadOnly);
			imgXlen = poSrcDS->GetRasterXSize();
			imgYlen = poSrcDS->GetRasterYSize();
			bandNum = poSrcDS->GetRasterCount();
		
			buffTmp = (GByte*)(CPLMalloc)(imgXlen*imgYlen*sizeof(GByte));

			M = new Pixel **[imgXlen];
			for(int i=0;i<imgXlen;i++){
				M[i] = new Pixel *[imgYlen];
				for(int j=0;j<imgYlen;j++){
					M[i][j] = new Pixel;
				}
			}
			duplication = GetGDALDriverManager()->GetDriverByName("MEM")->Create("",imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);

			for(int i = 0; i < bandNum; i++){
				poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,
				0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
				duplication->GetRasterBand(i+1)->RasterIO(GF_Write,
				0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
				for (int j=0;j<imgXlen;j++){
					for(int k=0;k<imgYlen;k++){
						M[j][k]->set(i,(unsigned short)buffTmp[j*imgYlen+k]);
					}
				}
			}
			ready = true;
		} else {
			cout <<"Open image failed."<<endl;
			path[0]='\0';
			imgXlen=0;
			imgYlen=0;
			bandNum=0;
			M=nullptr;
			buffTmp=nullptr;
			poSrcDS = nullptr;
			duplication = nullptr;
		}
	}
	
	unsigned short getXlen(){
		return imgXlen;
	}
	unsigned short getYlen(){
		return imgYlen;
	}
	unsigned short getBandNum(){
		return bandNum;
	}
	Pixel ***getPic(){
		return M;
	}
	Pixel getPixel(const short x, const short y)const{
		if (!in(x,0,imgYlen-1) || !in(y,0,imgYlen-1)){
			Pixel p;
			return p;
		}
		return *M[x][y];
	}
	bool load(const char *_path){
		
		return true;
	}

	bool save(const char* dstPath, const char* format="JPEG"){
		if (!ready)
			return false;
		if (strcmp(dstPath, path)==0){
			cout<<"Cannot over write source file."<<endl;
			return false;
		}
		GDALDriver *pDriverJPG = GetGDALDriverManager()->GetDriverByName(format);
		pDriverJPG->CreateCopy(dstPath,duplication,TRUE,0,0,0);
		return true;
	}

	bool update(){			
		for(int i = 0; i < bandNum; i++){
			for (int j=0;j<imgXlen;j++){
				for(int k=0;k<imgYlen;k++){
					buffTmp[j*imgYlen+k]=(GByte)(M[j][k]->getBand(i));
				}
			}
			duplication->GetRasterBand(i+1)->RasterIO(GF_Write,
			0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
		}
		return true;
	}

	bool Cov(Kernel *k){
		short xLen = (k->getXlen()-1)/2;
		short yLen = (k->getYlen()-1)/2;
		vector<vector<Pixel> > tmpM(imgXlen,vector<Pixel>(imgYlen));
		//Pixel **tmpM = new Pixel *[imgXlen];
		for (int i = 0 ;i< imgXlen ;i++){
			//tmpM[i] = new Pixel [imgYlen];
			for (int j = 0 ;j< imgYlen ;j++){
				for (int u = -xLen;u<=xLen;u++){
					for (int h = -yLen;h<=yLen;h++){
						tmpM[i][j].add(
							getPixel(i+u,j+h).muti(
								k->getScale(u+xLen,h+yLen)
								)
							);
					}
				}
				tmpM[i][j].cutOff();
			}
		}
		for (int i = 0 ;i< imgXlen ;i++){
			for (int j = 0 ;j< imgYlen ;j++){
				*M[i][j] = tmpM[i][j];
			}
		}
		update();
		//delete []tmpM;
		return true;
	}

	~Picture(){
		delete []M;
	    CPLFree(buffTmp);
		if(ready){
			GDALClose(poSrcDS);
			GDALClose(duplication);
		}
	}
};