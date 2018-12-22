// Task5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Pic.h"
#include <iostream>

using namespace std;

struct A{
	int a[4];
};

A t(){
	A ans;
	ans.a[0] = 0;
	ans.a[1] = 1;
	return ans;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	A q = t();
	cout<<q.a[0]<<" "<<q.a[1]<<" "<<q.a[2]<<endl;
	system("PAUSE");
	return 0;
	*/
	char *path1 = "American_MUL.bmp";
	char *path2 = "American_PAN.bmp";
	Picture P_MUL(path1);
	Picture P_PAN(path2);

	Picture* res = IHS(&P_MUL,&P_PAN);

	cout<<"done. "<<endl;

	res -> save("American_FUS.jpg");

	system("PAUSE");
	
	delete res;
	return 0;
}

