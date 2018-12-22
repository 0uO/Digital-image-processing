#include "stdafx.h"
#include "matrix.h"
#include<iostream>

using namespace std;
//----------------------------------------------------------------------------------

matrix::matrix(void)
{
	Row = 0;
	Col = 0;
	Size = Row * Col;
	Elem = NULL;
}

matrix::matrix(int Cols, int Rows, long double val)
{
	Row = Rows;
	Col = Cols;
	Size = Row * Col;
	Elem = new long double [Row * Col];
	for(int i = 0 ; i < Row * Col; i++)
		Elem[i] = val;
}

matrix::matrix(int lines, double val)
{
	Col = Row = lines;
	Size = Col * Row;
	Elem = new long double [Size];
	for(int i = 0 ; i < Col; i++)
	{
		for(int j = 0; j < Row; j++)
		{
			Elem[i * Row + j] = ( j == i ? val : 0.0);
		}
	}
}

matrix::matrix(int Cols, int Rows, long double* array)
{
	Col = Cols;
	Row = Rows;
	Size = Col * Row;
	Elem = new long double [Size];
	for(int i = 0; i < Size; i++)
	{
		//cannot promise len(array) <= matrix.Size
		Elem[i] = array[i];
	}
}

matrix::matrix(int Cols, int Rows, long double **Array)
{
	Col = Cols;
	Row = Rows;
	Size = Col * Row;
	Elem = new long double [Size];
	for(int i = 0; i < Col; i++)
		for(int j = 0; j < Row; j++)
			Elem[i * Row + j] = Array[i][j];
}

matrix::matrix(const matrix& M)
{
	Col = M.Col;
	Row = M.Row;
	Size = M.Size;
	Elem = new long double [M.Size];
	for(int i = 0; i < M.Size; i++)
		Elem[i] = M.Elem[i];
}

matrix::~matrix(void)
{
	delete []Elem;            
}
//-----------------------------------------------------------------------------------
long double& matrix::operator()(int L,int R)
{
	return Elem[(L - 1) * Row + (R - 1)];
}

matrix& matrix::operator=(const matrix& M) 
{

	if (this == &M)
	{
		return *this;
	}
	Col = M.Col;
	Row = M.Row;
	Size = M.Size;
	Elem = new long double [Size];
	for(int i = 0; i < Size; i++)
		Elem[i] = M.Elem[i];
	return *this;
}
 

matrix& matrix::operator+=(const matrix& M)
{
	if(Col == M.Col && Row == M.Row)
	{
		Elem = new long double [M.Size];
		for(int i = 0; i < M.Size; i++)
			Elem[i] += M.Elem[i];
		return *this;
	}
	else
	{
		std::cout << "cannot plus! (matrix::operator+)" << std::endl;
		return *this;
	}
}

matrix& matrix::operator+=(long double val)
{
	for(int i = 0; i < this->Size; i++)
		this->Elem[i] += val;
	return *this;
}
 
 
matrix& matrix::operator-=(const matrix& M)
{
	if(Col == M.Col && Row == M.Row)
	{
		Elem = new long double [M.Size];
		for(int i = 0; i < M.Size; i++)
			Elem[i] -= M.Elem[i];
		return *this;
	}
	else
	{
		std::cout << "cannot plus! (matrix::operator+)" << std::endl;
		return *this;
	}
}

matrix& matrix::operator-=(long double val)
{
	for(int i = 0; i < this->Size; i++)
		this->Elem[i] -= val;
	return *this;
}
 

matrix& matrix::operator*=(const matrix& M)
{
	if(Row == M.Col)
	{
		for(int i = 0; i < Col; i++)
			for(int j = 0; j < Row; j++)
			{
				long double temp = 0.0;
				for(int k = 0; k < Row; k++)
				{
					temp += GetElem(i+1,k+1) * M.Elem[k * M.Row + j];
				}
				SetElem(i+1,j+1,temp);
			}
		return *this;
	}
	else
	{
		std::cout << "cannot mutiply! (matrix::operator*)" << std::endl;
		return *this;
	}
}

matrix& matrix::operator*=(long double val)
{
	for(int i = 0; i < this->Size; i++)
		this->Elem[i] *= val;
	return *this;
}
 
 
matrix operator+(const matrix& matrix1, const matrix& matrix2)
{
	matrix temp(matrix1);
	if(matrix1.Col == matrix2.Col && matrix1.Row == matrix2.Row)
	{
		for(int i = 0; i < matrix1.Size; i++)
			temp.Elem[i] += matrix2.Elem[i];
		return temp;
	}
	else
	{
		std::cout << "canoot plus! (friend operator-)" << std::endl;
		return temp;
	}
}

matrix operator-(const matrix& matrix1, const matrix& matrix2)
{
	if(matrix1.Col == matrix2.Col && matrix1.Row == matrix2.Row)
	{
		for(int i = 0; i < matrix1.Size; i++)
			matrix1.Elem[i] += -(matrix2.Elem[i]);
		return matrix1;
	}
	else
	{
		std::cout << "cannot subtract! (friend operator-)" << std::endl;
		return matrix1;
	}
}

matrix operator*(const matrix& matrix1, const matrix& matrix2)
{
	matrix Temp(matrix1.Col,matrix2.Row);
	if(matrix1.Row == matrix2.Col)
	{
		for(int i = 0; i < Temp.Col; i++)
			for(int j = 0; j < Temp.Row; j++)
			{
				long double temp = 0.0;
				for(int k = 0; k < matrix1.Row; k++)
				{
					temp += matrix1.GetElem(i+1,k+1) * matrix2.GetElem(k+1,j+1);
				}
				Temp.SetElem(i+1,j+1,temp);
			}
		return Temp;
	}
	else
	{
		std::cout << "cannot mutiply! (matrix::operator*)" << std::endl;
		return matrix1;
	}
}

matrix operator+(const matrix& matrix,const long double val)
{
	for(int i=0; i < matrix.Size; i++)
		matrix.Elem[i] += val;
	return matrix;
}

matrix operator+(const long double val,const  matrix& matrix)
{
	return matrix + val;
}

matrix operator-(const matrix& matrix, const long double val)
{
	return (matrix + val) * (-1) ;
}

matrix operator-(const long double val, const matrix& matrix)
{
	return matrix + val;
}

matrix operator*(const matrix& matrix1,const long double val)
{
	for(int i = 0; i < matrix1.Size; i++)
		matrix1.Elem[i] = matrix1.Elem[i] * val;
	return matrix1;
}

matrix operator*(const long double val,const  matrix& matrix)
{
	return matrix * val;
}

std::ostream& operator<<(std::ostream& OS, const matrix& matrix)
{
	for(int i = 0; i < matrix.Col; i++)
	{
		for(int j = 0; j < matrix.Row; j++)
		{
			OS << matrix.Elem[i * matrix.Row + j]<< " ";
		}
		OS << std::endl;
	}
	return OS;
}
//------------------------------------------------------------------------------------

void matrix::rerange(int Cols, int Rows)
{
	if(Cols * Rows != Size)
	{
		std::cout << "Cols * Rows != Size (matrix::rerange)" << std::endl;
		return ;
	}
	else
	{
		Col = Cols;
		Row = Rows;
	}
}

void matrix::show()
{
	std::cout << *this;
}

matrix  matrix::T()
{
	matrix Temp(Row,Col);
	for(int i = 0; i < Temp.Col; i++)
		for(int j = 0; j < Temp.Row; j++)
			Temp.Elem[i * Temp.Row +j] = Elem[j * Temp.Col +i];
	Temp.rerange(Temp.Col,Temp.Row);
	return Temp;
}

long double matrix::GetElem(int L,int R)  const                  
{
	return Elem[(L - 1) * Row + (R - 1)];
}

void matrix::SetElem(int L, int R, long double val)         
{
	Elem[(L - 1) * Row + (R - 1)] = val;
}

matrix matrix::Inv()
{
	if(Col != Row)    //
	{
		std::cout << "not a square! (matrix::Inv)" << std::endl;
		exit(1);
	}
	matrix E(Col);    //
	double Max;        //
	int Max_Label;             //
	for(int i = 0; i < Col; i++)  //
	{
		Max = GetElem(i+1,i+1);
		Max_Label = 0;
		if(i < Col - 1)
		{
			for(int l = i + 1; l < Col; l++)   //
			{
				if(abs(Max) < abs(GetElem(l+1,i+1)))  //
				{
					Max = GetElem(l+1,i+1);
					Max_Label = l;
					for(int k = 1; k <= Col; k++)
					{
						long double* Swap = new long double;
						*Swap = GetElem(Max_Label+1,k);
						SetElem(Max_Label+1,k,GetElem(i+1,k));
						SetElem(i+1,k,*Swap);
						*Swap = E.GetElem(Max_Label+1,k);
						E.SetElem(Max_Label+1,k,E.GetElem(i+1,k));
						E.SetElem(i+1,k,*Swap);
						delete Swap;
					}
					Max = GetElem(i+1,i+1);
				}
			}
			if(Max == 0)
			{
				this->show();
				std::cout << "not full rank." << std::endl;
				exit(1);
			}
			//this->show();
			//E.show();
			//this->show();
			for(int l = 0; l < Col; l++)
			{
				if( l == i)
					continue;
				else
				{
					if(GetElem(l+1,i+1) != 0)
					{
						long double a = GetElem(l+1,i+1);
						for(int k = 0; k < Col; k++)
						{
							//long double temp = Max * GetElem(l+1,k+1)/a - GetElem(i+1,k+1);
							SetElem(l+1,k+1,Max * GetElem(l+1,k+1)/a - GetElem(i+1,k+1));
							E.SetElem(l+1,k+1,Max * E.GetElem(l+1,k+1)/a - E.GetElem(i+1,k+1));
						}
 
					}
				}
			}
		}
		else
		{
			for(int j = 1; j <= Col -1; j++)
			{
				long double a = GetElem(j,Col);
				if(abs(a) < 1.0E-016)
					continue;
				else
				{
					for(int k = 1; k <= Col; k++)
					{
						SetElem(j,k,Max * GetElem(j,k)/a - GetElem(Col,k));
						E.SetElem(j,k,Max * E.GetElem(j,k)/a -  E.GetElem(Col,k));
					}
				}
			}
		}
	}

	for(int i = 1; i <= Col; i++)
	{
		long double a = GetElem(i,i);
		for(int j = 1; j <= Col; j++)
		{
			SetElem(i,j,GetElem(i,j)/a);
			E.SetElem(i,j,E.GetElem(i,j)/a);
		}
	}
	return E;
}