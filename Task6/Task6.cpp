#include "stdafx.h"
#include "Pic.h"
#include <iostream>
#include <time.h> 

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//char *pathMul = "Mul_large.tif";
	//char *pathPan = "Pan_large.tif";
	char *pathMul = "American_MUL.bmp";
	char *pathPan = "American_PAN.bmp";
	
	char *dst_path = "ans_example.tif";
	
	/*
	LargePicture* im_fus = new LargePicture(1000,1000,3,0,4,dst_path);
	Pixel ***_fus = im_fus->getPic();
	int x = 0;
	do{
		x++;
		for(int i =0; i < im_fus->blockXlen;i++){
			for(int j =0; j < im_fus->blockYlen;j++){
				_fus[i][j]->set(0,x);
			}
		}
		im_fus->update();
	}while(im_fus->next());
	delete im_fus;
	
	system("PAUSE");
	return 0;
	*/
	
	//Picture PMul(pathMul);
	//Picture PPan(pathPan);

	clock_t  ti = clock();
	//Picture* ans = IHS(&PMul,&PPan);
	//ans->save("ans_pic.tif","GTiff");
	//delete ans;
	//Picture ans(pathMul);
	//ans.save("ans_pic.tif","GTiff");


	///*
	LargePicture PMul(pathMul,0,256);
	LargePicture PPan(pathPan,0,256);
	LargePicture* ans = IHS(&PMul,&PPan,dst_path);
	delete ans;
	Picture tmp(dst_path);
	tmp.save("ans_example.jpg","JPEG");
	///*

	cout<<(float)(clock()-ti)/(float)1000<<endl;
	system("PAUSE");
	
	return 0;
}

