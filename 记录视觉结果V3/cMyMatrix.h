#pragma once
extern "C"
{
#include <my++\myhead.h>
}

class cMyMatrix
{
public:
	cMyMatrix();
	~cMyMatrix();
	void InitMyMatrix(int row, int colum);
	//bool InitMyMatrix(double* data, int row, int colum);

	int mRow;
	int mColum;

	

	cMyMatrix operator = (const cMyMatrix &A);
	cMyMatrix operator + (const cMyMatrix &A);
	cMyMatrix operator - (const cMyMatrix &A);
	cMyMatrix operator * (const cMyMatrix &A);
	/*cMyMatrix cMyMatrix::operator + (cMyMatrix &A);*/
	
	cMyMatrix Transpose();
	cMyMatrix Inv();
	//============================================
	Matrix * mat;

};

