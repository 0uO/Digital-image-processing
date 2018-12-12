// Task2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>

using namespace std;

#include "gdal\gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	GDALDataset* poSrcDS;
    GDALDataset* poDstDS;
    int imgXlen, imgYlen;
    char srcPath[32] = "test.jpg";
    char dstPath[32] = "test.tif";
    GByte* buffTmp;
    int i,j,k,bandNum;

    GDALAllRegister();

	// 读源图像
    poSrcDS = (GDALDataset*)GDALOpenShared(srcPath,GA_ReadOnly);

    imgXlen = poSrcDS->GetRasterXSize();
    imgYlen = poSrcDS->GetRasterYSize();
    bandNum = poSrcDS->GetRasterCount();

    cout<<imgXlen<<endl;
    cout<<imgYlen<<endl;
    cout<<bandNum<<endl;

	// 写原图像
    buffTmp = (GByte*)(CPLMalloc)(imgXlen*imgYlen*sizeof(GByte));

    poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath,imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);

    for(i = 0; i < bandNum; i++){
        poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,
        0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
        poDstDS->GetRasterBand(i+1)->RasterIO(GF_Write,
        0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
        cout<<"... band "<<i<<" processed ..."<<endl;
    }
	
	//  任务1
	
	int StartX=100, StartY=100,tmpXlen=200,tmpYlen=150;
    buffTmp = (GByte*)(CPLMalloc)(tmpXlen*tmpXlen*sizeof(GByte));
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);

	for(j=0;j<tmpYlen;j++){
		for(i=0;i<tmpXlen;i++){
			buffTmp[j*tmpXlen+i] = (GByte)255;
		}
	}
	
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);

	// 任务2
	
    CPLFree(buffTmp);
	StartX=300, StartY=300,tmpXlen=100,tmpYlen=50;
    buffTmp = (GByte*)(CPLMalloc)(tmpXlen*tmpXlen*sizeof(GByte));
	
	for(j=0;j<tmpYlen;j++){ 
		for(i=0;i<tmpXlen;i++){
			buffTmp[j*tmpXlen+i] = (GByte)255;
		}
	}
	for(k=1;k<4;k++){
		//poSrcDS->GetRasterBand(k)->RasterIO(GF_Read,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);
	}

    CPLFree(buffTmp);
	StartX=500, StartY=500,tmpXlen=50,tmpYlen=100;
    buffTmp = (GByte*)(CPLMalloc)(tmpXlen*tmpYlen*sizeof(GByte));
		
	for(j=0;j<tmpYlen;j++){
		for(i=0;i<tmpXlen;i++){
			buffTmp[j*tmpXlen+i] = (GByte)0;
		}
	}
	for(k=1;k<4;k++){
		//poSrcDS->GetRasterBand(k)->RasterIO(GF_Read,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write,StartX,StartY,tmpXlen,tmpYlen,buffTmp,tmpXlen,tmpYlen,GDT_Byte,0,0);
	}

    CPLFree(buffTmp);
    GDALClose(poDstDS);
    GDALClose(poSrcDS);
	system("PAUSE");

	return 0;
}

