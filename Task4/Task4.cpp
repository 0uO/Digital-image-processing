// Task4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Pic.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	char *path = "face.jpg";
	Picture P(path);
	const int r = 3, c = 3;

	vector<vector<float> > m1(r,vector<float> (c));
	
	float tmp[r][c] = {
		{-1,-1,-1},
		{-1,8,-1},
		{-1,-1,-1}
	};

	for (int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			m1[i][j]=tmp[i][j];
			cout << m1[i][j]<<" ";
		}
		cout<<endl;
	}

	Kernel *k1 = new Kernel(m1);

	P.Cov(k1);
	P.save("output3.jpg");	
	
	system("PAUSE");
	return 0;
}

