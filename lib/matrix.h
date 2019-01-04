#pragma once
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
class matrix
{
private:
	int Col;           //
	int Row;            //
	int Size;           //
	long double* Elem;  //
public:
	matrix(void);//
	matrix(int Cols, int Rows, long double Val = 0);   //
	matrix(int Cols, double Val = 1);                  //
	matrix(int Cols, int Rows, long double * Array);
	matrix(int Cols, int Rows, long double **Array);
	matrix(const matrix& matrix);                       //
	void show();                                        //
	void rerange(int Cols,int Rows);                    //
	long double GetElem(int L,int R) const;             //
	void SetElem(int L, int R, long double val);        //
	int GetCols() const { return Col; };                //
	int GetRows() const { return Row; };                //
	int GetSize() const { return Row * Col; };          //
 
	long double& operator()(int L,int R);
 
	matrix  T();                                        //
	matrix  Inv();                                      //
	matrix& operator=(const matrix& matrix);            //
	matrix& operator+=(const matrix& matrix2);          //
	matrix& operator+=(long double val);                //
	matrix& operator-=(const matrix& matrix2);          //
	matrix& operator-=(long double val);                //
	matrix& operator*=(const matrix& matrix2);          //do not change Row & Col
	matrix& operator*=(long double val);                //
	
	~matrix(void);
public:
	friend matrix  operator+(const matrix& matrix1,const matrix& matrix2);//
	friend matrix  operator-(const matrix& matrix1,const matrix& matrix2);//
	friend matrix  operator*(const matrix& matrix1,const matrix& matrix2);//
	friend matrix  operator+(const matrix& matrix1,const long double val);      //
	friend matrix  operator+(const long double val,const matrix& matrix1);      //
	friend matrix  operator-(const matrix& matrix1,const long double val);      //
	friend matrix  operator-(const long double val,const matrix& matrix1);      //
	friend matrix  operator*(const matrix& matrix1,const long double val);      //
	friend matrix  operator*(const long double val,const matrix& matrix1);      //
	friend std::ostream& operator<<(std::ostream &os, const matrix& matrix); //
};
