#include "stdafx.h"
#include "gdal\gdal_priv.h"
#include "Pic.h"
#include "matrix.h"
#include<iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>
#include <time.h> 
#pragma comment(lib, "gdal_i.lib")

using namespace std;

bool assert(const bool f, const char * s){
	if (!f){
		cout<<"Error occured: "<<s<<endl;
		system("PAUSE");
		exit(1);
	}
	return f;
}

bool existFile(const char *_path){
	if (_path == nullptr)
		return false;
	fstream _file;
	_file.open(_path,ios::in);
	if(!_file)
		return false;
	return true;
}	

bool in(const int x,const int l,const int r){
		return x>=l && x<=r;
	}

//-------------------------------------------------------------------------------------

	Pixel::Pixel(const float _r, const float _g, const float _b):r(_r),g(_g),b(_b){}
	Pixel::Pixel(const Pixel &p){
		r = p.r;
		g = p.g;
		b = p.b;
	}

	bool Pixel::set(const unsigned short channal, const float n){
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
	bool Pixel::set(const Pixel *p){
		r = p->getR();
		g = p->getG();
		b = p->getB();
		return true;
	}
	bool Pixel::set(const Pixel p){
		r = p.getR();
		g = p.getG();
		b = p.getB();
		return true;
	}

	float Pixel::getR()const {
		return r;
	}
	float Pixel::getG()const {
		return g;
	}
	float Pixel::getB()const {
		return b;
	}
	float Pixel::getBand(const unsigned short channal)const {
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
	
	bool Pixel::cutOff(){
		if(r > 255) r=255;
		if(g > 255) r=255;
		if(b > 255) r=255;
		if(r <0) r=0;
		if(g <0) g=0;
		if(b <0) b=0;
		return true;
	}
	
	bool Pixel::add(const Pixel *p) {
		r += p->getR();
		g += p->getG();
		b += p->getB();
		return true;
	}	
	bool Pixel::add(const Pixel p) {
		r += p.getR();
		g += p.getG();
		b += p.getB();
		return true;
	}

	Pixel Pixel::plus(const Pixel *p)const{
		Pixel tmpP = *this;
		tmpP.r += p->getR();
		tmpP.g += p->getG();
		tmpP.b += p->getB();
		return tmpP;
	}

	Pixel Pixel::muti(float f)const{
		Pixel tmpP;
		tmpP.r = (short)((float)r*f);
		tmpP.g = (short)((float)g*f);
		tmpP.b = (short)((float)b*f);
		return tmpP;
	}

	void Pixel::print()const {
		cout<<"r: "<<r<<"    g: "<<g<<"    b: "<<b<<" | ";
	}

//-------------------------------------------------------------------------------------

	Kernel::Kernel(unsigned short _Xlen, unsigned short _Ylen, float **_M){
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
	Kernel::Kernel(vector<vector<float> > _M){
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

	bool Kernel::set(float **_M){
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

	unsigned short Kernel::getXlen()const{
		return Xlen;
	}
	unsigned short Kernel::getYlen()const{
		return Ylen;
	}
	float Kernel::getScale(const short x,const short y)const{
		return M[x][y];
	}

	Kernel::~Kernel(){
		delete []M;
	}

//-------------------------------------------------------------------------------------

	metaPicture::metaPicture(){}
	
	unsigned int metaPicture::getXlen()const{
		return imgXlen;
	}
	unsigned int metaPicture::getYlen()const{
		return imgYlen;
	}
	unsigned short metaPicture::getBandNum()const{
		return bandNum;
	}
	Pixel*** metaPicture::getPic()const{
		return M;
	}
	Pixel metaPicture::getPixel(const int x, const int y)const{
		if (!in(x,0,imgYlen-1) || !in(y,0,imgYlen-1)){
			Pixel p;
			return p;
		}
		return *M[x][y];
	}
	
	bool metaPicture::update(){		
		assert(duplication != nullptr, "Error: duplication == nullptr ;");
		for(int i = 0; i < bandNum; i++){
			for (int j=0;j<MXSize;j++){
				for(int k=0;k<MYSize;k++){
					buffTmp[j*MYSize+k]=(GByte)(M[j][k]->getBand(i));
				}
			}
			duplication->GetRasterBand(i+1)->RasterIO(GF_Write,
			0,0,MXSize,MYSize,buffTmp,MXSize,MYSize,GDT_Byte,0,0);
		}
		return true;
	}

	metaPicture::~metaPicture(){}

//-------------------------------------------------------------------------------------

	Picture::Picture(const char *_path){
		
		GDALAllRegister();
		ready = false;

		if (existFile(_path)){
			strcpy_s(path, _path);
			poSrcDS = (GDALDataset*)GDALOpenShared(path,GA_ReadOnly);
			imgXlen = poSrcDS->GetRasterXSize();
			imgYlen = poSrcDS->GetRasterYSize();
			bandNum = poSrcDS->GetRasterCount();

			duplication = GetGDALDriverManager()->GetDriverByName("MEM")->Create("",imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);
			
			buffTmp = (GByte*)(CPLMalloc)(imgXlen*imgYlen*sizeof(GByte));
			M = new Pixel **[imgXlen];
			MXSize = imgXlen;
			MYSize = imgYlen;
			for(unsigned int i=0;i<imgXlen;i++){
				M[i] = new Pixel *[imgYlen];
				for(unsigned int j=0;j<imgYlen;j++){
					M[i][j] = new Pixel;
				}
			}
			for(unsigned short i = 0; i < bandNum; i++){
				poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,
				0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
				duplication->GetRasterBand(i+1)->RasterIO(GF_Write,
				0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
				for (unsigned int j=0;j<imgXlen;j++){
					for(unsigned int k=0;k<imgYlen;k++){
						M[j][k]->set(i,buffTmp[j*imgYlen+k]);
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
	Picture::Picture(const unsigned int x, const unsigned int y, const unsigned short b){
			
		GDALAllRegister();
		ready = false;

		poSrcDS = nullptr;
		imgXlen = x;
		imgYlen = y;
		bandNum = b;
		
		duplication = GetGDALDriverManager()->GetDriverByName("MEM")->Create("",imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);

		buffTmp = (GByte*)(CPLMalloc)(imgXlen*imgYlen*sizeof(GByte));

		M = new Pixel **[imgXlen];
		MXSize = imgXlen;
		MYSize = imgYlen;
		for(unsigned int i=0;i<imgXlen;i++){
			M[i] = new Pixel *[imgYlen];
			for(unsigned int j=0;j<imgYlen;j++){
				M[i][j] = new Pixel;
			}
		}

		for(unsigned short i = 0; i < bandNum; i++){
			for (unsigned int j=0;j<imgXlen;j++){
				for(unsigned int k=0;k<imgYlen;k++){
					M[j][k]->set(i,0);
				}
			}
		}
		ready = true;
		cout<<"default picture set."<<endl;
	}

	bool Picture::save(const char* dstPath, const char* format){
		if (!ready)
			return false;
		if (strcmp(dstPath, path)==0){
			cout<<"Cannot over write source file."<<endl;
			return false;
		}    
		char **papszOptions = NULL;
		papszOptions = CSLSetNameValue( papszOptions, "TILED", "YES" );
		papszOptions = CSLSetNameValue( papszOptions, "COMPRESS" ,"PACKBITS" );
		GDALDriver *pDriverJPG = GetGDALDriverManager()->GetDriverByName(format);
		pDriverJPG->CreateCopy(dstPath,duplication,FALSE,0, GDALTermProgress,0);
		return true;
	}

	bool Picture::Cov(Kernel *k){
		short xLen = (k->getXlen()-1)/2;
		short yLen = (k->getYlen()-1)/2;
		vector<vector<Pixel> > tmpM(imgXlen,vector<Pixel>(imgYlen));

		for (unsigned int i = 0 ;i< imgXlen ;i++){
			for (unsigned int j = 0 ;j< imgYlen ;j++){
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
		for (unsigned int i = 0 ;i< imgXlen ;i++){
			for (unsigned int j = 0 ;j< imgYlen ;j++){
				*M[i][j] = tmpM[i][j];
			}
		}
		update();

		return true;
	}

	Picture::~Picture(){
		delete []M;
	    CPLFree(buffTmp);
		if(ready){
			if(poSrcDS != nullptr)
				GDALClose(poSrcDS);
			GDALClose(duplication);
		}
	}


Picture* IHS(Picture* P_MUL, Picture* P_PAN){

	const unsigned short Xlen = P_MUL->getXlen(),Ylen = P_MUL->getYlen(), bandNum = 3;

	cout<<Xlen<<"	"<<Ylen<<endl;
	cout<<P_PAN->getXlen()<<"	"<<P_PAN->getYlen()<<endl;
	cout<<P_MUL->getBandNum()<<endl;

	assert( Xlen == P_PAN->getXlen() && Ylen == P_PAN->getYlen() );
	assert( (P_MUL->getBandNum()) == bandNum );
	
	Picture* im_fus = new Picture(Xlen,Ylen);
	Pixel ***M_MUL = P_MUL->getPic();
	Pixel ***M_PAN = P_PAN->getPic();
	Pixel ***M_fus = im_fus->getPic();

	float bandH, bandS;

	for(int i = 0 ; i< Xlen; i++){
		for (int j = 0 ; j < Ylen ;j++){
			bandH = -sqrt(2.0f)/6.0f*M_MUL[i][j]->getR()-sqrt(2.0f)/6.0f*M_MUL[i][j]->getG()+sqrt(2.0f)/3.0f*M_MUL[i][j]->getB();
			bandS = 1.0f/sqrt(2.0f)*M_MUL[i][j]->getR()-1/sqrt(2.0f)*M_MUL[i][j]->getG();
			
			M_fus[i][j] -> set(0,M_PAN[i][j]->getR()-1.0f/sqrt(2.0f)*bandH+1.0f/sqrt(2.0f)*bandS);
			M_fus[i][j] -> set(1,M_PAN[i][j]->getR()-1.0f/sqrt(2.0f)*bandH-1.0f/sqrt(2.0f)*bandS);
			M_fus[i][j] -> set(2,M_PAN[i][j]->getR()+sqrt(2.0f)*bandH);
			M_fus[i][j] -> cutOff();
		}
	}

	im_fus->update();

	return im_fus;
}
	
//-------------------------------------------------------------------------------------
	LargePicture::LargePicture(const char *_path, const unsigned short B_X, const unsigned short B_Y, const char *dst_path):blockXlen(B_X),blockYlen(B_Y){

		GDALAllRegister();
		ready = false;

		if (existFile(_path)){
			strcpy_s(path, _path);
			poSrcDS = (GDALDataset*)GDALOpenShared(path,GA_ReadOnly);
			imgXlen = poSrcDS->GetRasterXSize();
			imgYlen = poSrcDS->GetRasterYSize();
			bandNum = poSrcDS->GetRasterCount();
			pointerX = 0;
			pointerY = 0;
			if(blockXlen==0)blockXlen = imgXlen;
			if(blockYlen==0)blockYlen = imgYlen;
			
			buffTmp = (GByte*)(CPLMalloc)(blockXlen*blockYlen*sizeof(GByte));
			if(strcmp(dst_path, "\0")!=0)
				duplication = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dst_path,imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);
			else duplication = nullptr;

			M = new Pixel **[imgXlen];
			MXSize = blockXlen;
			MYSize = blockYlen;
			for(unsigned int i=0;i<blockXlen;i++){
				M[i] = new Pixel *[blockYlen];
				for(unsigned int j=0;j<blockYlen;j++){
					M[i][j] = new Pixel;
				}
			}
			unsigned short loadSizeX = nextLoadSizeX();
			unsigned short loadSizeY = nextLoadSizeY();
			for(unsigned short i = 0;i<bandNum;i++){
				poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,pointerX,pointerY,
					loadSizeX,loadSizeY,buffTmp,loadSizeX,loadSizeY,GDT_Byte,0,0); // ??? can I do this?
				for (unsigned int j=0;j<MXSize;j++){
					for(unsigned int k=0;k<MYSize;k++){
						M[j][k]->set(i,buffTmp[j*MYSize+k]);
					}
				}
			}

			ready = true;
			cout<<path<<" loaded."<<endl;
		
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
	LargePicture::LargePicture(const unsigned int x, const unsigned int y, const unsigned short b, const unsigned short B_X, const unsigned short B_Y, const char *dst_path):blockXlen(B_X),blockYlen(B_Y){		
		
		GDALAllRegister();
		ready = false;

		poSrcDS = nullptr;
		imgXlen = x;
		imgYlen = y;
		bandNum = b;
		pointerX = 0;
		pointerY = 0;
		if(blockXlen==0)blockXlen = imgXlen;
		if(blockYlen==0)blockYlen = imgYlen;

		cout<<imgXlen<<" X "<<imgYlen<<" X "<<bandNum<<endl;

		buffTmp = (GByte*)(CPLMalloc)(blockXlen*blockYlen*sizeof(GByte));
		if(strcmp(dst_path, "\0")!=0)
				duplication = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dst_path,imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);
		else duplication = nullptr;

		M = new Pixel **[imgXlen];
		MXSize = blockXlen;
		MYSize = blockYlen;
		for(unsigned int i=0;i<blockXlen;i++){
			M[i] = new Pixel *[blockYlen];
			for(unsigned int j=0;j<blockYlen;j++){
				M[i][j] = new Pixel;
			}
		}
		for(unsigned short i = 0; i < bandNum; i++){
			for (unsigned int j=0;j<MXSize;j++){
				for(unsigned int k=0;k<MYSize;k++){
					M[j][k]->set(i,0);
				}
			}
		}

		ready = true;

		cout<<"default large picture set."<<endl;
	}
	
	unsigned short LargePicture::getBlockXlen()const{return blockXlen;}
	unsigned short LargePicture::getBlockYlen()const{return blockYlen;}
	unsigned int LargePicture::getXpointer()const{
		return pointerX;
	}
	unsigned int LargePicture::getYpointer()const{
		return pointerY;
	}

	unsigned short LargePicture::nextLoadSizeX(){
		unsigned short loadSizeX = blockXlen;		
		if(pointerX + loadSizeX > imgXlen ){
			loadSizeX = imgXlen - pointerX;
		}
		return loadSizeX;
	}	
	unsigned short LargePicture::nextLoadSizeY(){
		unsigned short loadSizeY = blockYlen;		
		if(pointerY + loadSizeY > imgYlen ){
			loadSizeY = imgYlen - pointerY;
		}
		return loadSizeY;
	}
	bool LargePicture::nextPointer(){
		if(pointerX == imgXlen){
			return false;
		}
		unsigned short loadSizeX = nextLoadSizeX();
		unsigned short loadSizeY = nextLoadSizeY();
		pointerY = pointerY + loadSizeY;
		if(pointerY == imgYlen){
			pointerY = 0;
			pointerX = pointerX + loadSizeX;
		}
		return true;
	}
	bool LargePicture::next(){
		if(nextPointer() == false)
			return false;
		
		unsigned short loadSizeX = nextLoadSizeX(), loadSizeY = nextLoadSizeY();
		for(unsigned short i = 0;i<bandNum;i++){
			if(poSrcDS!=nullptr)
				poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,pointerX,pointerY,
					loadSizeX,loadSizeY,buffTmp,loadSizeX,loadSizeY,GDT_Byte,0,0); // ??? can I do this?
			else memset(buffTmp,0,sizeof buffTmp);
			for (unsigned int j=0;j<MXSize;j++){
				for(unsigned int k=0;k<MYSize;k++){
					M[j][k]->set(i,buffTmp[j*MYSize+k]);
				}
			}
		}

		return true;
	}
	void LargePicture::reset(){
		pointerX = 0;
		pointerY = 0;
	}

	bool LargePicture::update(){		
		assert(duplication != nullptr, "Error: duplication == nullptr ;");
		unsigned  short loadSizeX = nextLoadSizeX();
		unsigned  short loadSizeY = nextLoadSizeY();
		for(int i = 0; i < bandNum; i++){
			for (int j=0;j<MXSize;j++){
				for(int k=0;k<MYSize;k++){
					buffTmp[j*MYSize+k]=(GByte)(M[j][k]->getBand(i));
				}
			}
			duplication->GetRasterBand(i+1)->RasterIO(GF_Write,pointerX,pointerY,
				loadSizeX,loadSizeY,buffTmp,loadSizeX,loadSizeY,GDT_Byte,0,0);
		}
		return true;
	}

	LargePicture::~LargePicture(){
		delete []M;
	    CPLFree(buffTmp);
		if(ready){
			if(poSrcDS != nullptr)
				GDALClose(poSrcDS);
			if(duplication!=nullptr)
				GDALClose(duplication);
		}
	}

	
LargePicture* IHS(LargePicture* P_MUL, LargePicture* P_PAN, const char* dst_file){

	const unsigned int Xlen = P_MUL->getXlen(), Ylen = P_MUL->getYlen();
	const unsigned short bandNum = 3,MXSize = P_MUL->getBlockXlen(),MYSize = P_MUL->getBlockYlen();
	
	assert( Xlen == P_PAN->getXlen() && Ylen == P_PAN->getYlen() && P_MUL->getBandNum() == bandNum, "IHS£º Size Error.");
	assert( MXSize== P_PAN->getBlockXlen() && MYSize == P_PAN->getBlockYlen(), "IHS£º Block Size Error.");

	LargePicture* im_fus = new LargePicture(Xlen,Ylen,bandNum,MXSize,MYSize,dst_file);
	Pixel ***M_fus = im_fus->getPic();
	Pixel ***M_MUL = P_MUL->getPic();
	Pixel ***M_PAN = P_PAN->getPic();

	float bandH, bandS;
	
	clock_t  ti = clock();

	do {
		for(unsigned int i = 0 ; i < MXSize; i++){
			for (unsigned int j = 0 ; j < MYSize ;j++){
				bandH = -sqrt(2.0f)/6.0f*(M_MUL[i][j]->getR())-sqrt(2.0f)/6.0f*M_MUL[i][j]->getG()+sqrt(2.0f)/3.0f*M_MUL[i][j]->getB();
				bandS = 1.0f/sqrt(2.0f)*(M_MUL[i][j]->getR())-1/sqrt(2.0f)*M_MUL[i][j]->getG();
				M_fus[i][j] -> set(0,M_PAN[i][j]->getR()-1.0f/sqrt(2.0f)*bandH+1.0f/sqrt(2.0f)*bandS);
				M_fus[i][j] -> set(1,M_PAN[i][j]->getR()-1.0f/sqrt(2.0f)*bandH-1.0f/sqrt(2.0f)*bandS);
				M_fus[i][j] -> set(2,M_PAN[i][j]->getR()+sqrt(2.0f)*bandH);
				M_fus[i][j] -> cutOff();
			}
		}

		im_fus->update();

		cout<<im_fus->getXpointer()<<"	"<<im_fus->getYpointer()<<endl;
		cout<<(float)(clock()-ti)/(float)1000<<endl;
	
	} while( im_fus->next() && P_MUL->next() && P_PAN->next() );
	
	return im_fus;
}

