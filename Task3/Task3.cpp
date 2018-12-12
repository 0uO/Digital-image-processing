// Task2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "Pic.h"

using namespace std;

bool in ( const unsigned short x, const unsigned short l,  const unsigned short r){
	return  x>l && x<r ;
}

int _tmain(int argc, _TCHAR* argv[])
{
    char *bgPath = "background.jpg";
    char *smPath = "superman.jpg";
	
	Picture *bg = new Picture(bgPath);
	Picture *sm = new Picture(smPath);
	
	Pixel ***bgM = bg->getPic();
	Pixel ***smM = sm->getPic();
	
	unsigned short Xlen=bg->getXlen();
	unsigned short Ylen=bg->getYlen();
	unsigned short bandNum=bg->getYlen();
	
	for(int i = 0; i < Xlen;i++){
		for(int j = 0; j < Ylen;j++){
			if((in(smM[i][j]->getR(),10,160)&&
				in(smM[i][j]->getG(),100,220)&&
				in(smM[i][j]->getB(),10,110))){
				bgM[i][j]->set(smM[i][j]);
			}
		}
	}

	bg->modify();
	bg->save("output2.jpg");

	system("PAUSE");

	return 0;
}

