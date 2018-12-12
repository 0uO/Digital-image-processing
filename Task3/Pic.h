#include<iostream>
#include "gdal\gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
#include <fstream>

using namespace std;

bool assert(const bool f){
	if (!f){
		system("PAUSE");
		cout<<"Error occured."<<endl;
	}
	return f;
}

class Pixel{
public:
	unsigned short r,g,b;
	Pixel(const unsigned short _r=0, const unsigned short _g=0, const unsigned short _b=0):r(_r),g(_g),b(_b){}
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

	bool set(Pixel *p){
		r = p->getR();
		g = p->getG();
		b = p->getB();
		return true;
	}

	unsigned short getR(){
		return r;
	}
	unsigned short getG(){
		return g;
	}
	unsigned short getB(){
		return b;
	}
	unsigned short getBand(const unsigned short channal){
		if(channal==0){
			return r;
		} else if(channal==1){
			return g;
		} else if(channal==2){
			return b;
		} else {
			return 0;
		}
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
		fstream _file;
		_file.open(_path,ios::in);
		if(!_file)
			return false;
		return true;
	}

	Picture(const char *_path = nullptr){
		
		GDALAllRegister();
		ready = false;

		if (_path!=nullptr && existFile(_path)){

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
			path[0]='\0';
			imgXlen=0;
			imgYlen=0;
			bandNum=0;
			M=nullptr;
			buffTmp=nullptr;
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

	bool modify(){			
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

	~Picture(){
		delete []M;
	    CPLFree(buffTmp);
		GDALClose(poSrcDS);
		GDALClose(duplication);
	}
};