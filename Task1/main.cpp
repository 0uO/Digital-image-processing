#include <iostream>
using namespace std;

#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main(){

    GDALDataset* poSrcDS;
    GDALDataset* poDstDS;
    int imgXlen, imgYlen;
    char srcPath[32] = "trees.jpg";
    char dstPath[32] = "trees.tif";
    GByte* buffTmp;
    int i,bandNum;

    GDALAllRegister();
    poSrcDS = (GDALDataset*)GDALOpenShared(srcPath,GA_ReadOnly);

    imgXlen = poSrcDS->GetRasterXSize();
    imgYlen = poSrcDS->GetRasterYSize();
    bandNum = poSrcDS->GetRasterCount();

    cout<<imgXlen;
    cout<<imgYlen;
    cout<<bandNum;

    buffTmp = (GByte*)(CPLMalloc)(imgXlen*imgXlen*sizeof(GByte));

    poDstDS = GetGDALDriverManager()->GetDriverByName(
        "GTiff")->Create(dstPath,imgXlen,imgYlen,bandNum,GDT_Byte,nullptr);

    for(i = 0; i < bandNum; i++){
        poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,
        0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
        poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Write,
        0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
        cout<<"... band "<<i<<" processed ...";
    }

    CPLFree(buffTmp);
    GDALClose(poDstDS);
    GDALClose(poSrcDS);

    return 0;
}
